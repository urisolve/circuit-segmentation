/**
 * @file
 */

#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include "schematicSegmentation/ComponentSegmentation.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace circuitSegmentation;
using namespace circuitSegmentation::computerVision;

/**
 * @brief Test class of ComponentSegmentation.
 */
class ComponentSegmentationTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockOpenCvWrapper = std::make_shared<NiceMock<MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);

        mComponentSegmentation
            = std::make_unique<schematicSegmentation::ComponentSegmentation>(mMockOpenCvWrapper, mLogger);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /**
     * @brief Expects calls to morphological operations.
     */
    void expectMorphOperations()
    {
        // Setup expectations
        EXPECT_CALL(*mMockOpenCvWrapper, getStructuringElement(OpenCvWrapper::MorphShapes::MORPH_RECT, _)).Times(2);
        EXPECT_CALL(*mMockOpenCvWrapper, morphologyEx(_, _, OpenCvWrapper::MorphTypes::MORPH_CLOSE, _, _)).Times(1);
        EXPECT_CALL(*mMockOpenCvWrapper, morphologyEx(_, _, OpenCvWrapper::MorphTypes::MORPH_OPEN, _, _)).Times(1);
    }

    /**
     * @brief Sets the behavior when finding contours.
     *
     * @param numberContours Number of contours found.
     */
    void onFindContours(const unsigned int numberContours)
    {
        ON_CALL(*mMockOpenCvWrapper, findContours)
            .WillByDefault([numberContours]([[maybe_unused]] ImageMat& image,
                                            Contours& contours,
                                            [[maybe_unused]] ContoursHierarchy& hierarchy,
                                            [[maybe_unused]] const OpenCvWrapper::RetrievalModes& mode,
                                            [[maybe_unused]] const OpenCvWrapper::ContourApproximationModes& method) {
                for (unsigned int i{0}; i < numberContours; ++i) {
                    Contour contour{};
                    contours.push_back(contour);
                }
            });
    }

    /**
     * @brief Sets up detection of components.
     *
     * @param componentsDetected Number of components detected.
     */
    void setupDetectComponents(const unsigned int componentsDetected)
    {
        constexpr auto imgWidth{100};
        constexpr auto imgHeight{100};
        // Area is larger than the minimum to consider blob as a component, if there are components to detect
        const auto contArea{componentsDetected > 0
                                ? schematicSegmentation::ComponentSegmentation::cBoundingBoxMinArea
                                : (schematicSegmentation::ComponentSegmentation::cBoundingBoxMinArea - 1)};
        const Rectangle rectangle{0, 0, 10, 10};

        // Setup expectations and behavior
        expectMorphOperations();
        onFindContours(componentsDetected);

        EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
        EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
        EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
        EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(componentsDetected).WillRepeatedly(Return(contArea));
        EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(componentsDetected).WillRepeatedly(Return(rectangle));
    }

    /**
     * @brief Sets up a dummy component.
     *
     * @param x X coordinate for component box.
     * @param y Y coordinate for component box.
     */
    void setupDummyComponent(const int& x, const int& y)
    {
        // Bounding box
        const Rectangle rectangle{x, y, cDimension, cDimension};

        // Dummy component
        circuit::Component dummyComponent{};
        dummyComponent.mBoundingBox = rectangle;

        // Set
        mDummyComponents.push_back(dummyComponent);
    }

    /**
     * @brief Sets up a dummy connection.
     *
     * @param x X coordinate for a point of connection wire.
     * @param y Y coordinate for a point of connection wire.
     */
    void setupDummyConnection(const int& x, const int& y)
    {
        // Wire
        const circuit::Wire wire{{x, y}};

        // Dummy connection
        circuit::Connection dummyConnection{};
        dummyConnection.mWire = wire;

        // Set
        mDummyConnections.push_back(dummyConnection);
    }

    /**
     * @brief Sets the behavior when checking if a rectangle contains a given point.
     */
    void onCheckContainsPoint()
    {
        ON_CALL(*mMockOpenCvWrapper, contains).WillByDefault([](const Rectangle& rectangle, const Point& point) {
            return rectangle.x <= point.x && point.x < rectangle.x + rectangle.width && rectangle.y <= point.y
                   && point.y < rectangle.y + rectangle.height;
        });
    }

protected:
    /** Component segmentation. */
    std::unique_ptr<schematicSegmentation::ComponentSegmentation> mComponentSegmentation;
    /** OpenCV wrapper. */
    std::shared_ptr<NiceMock<MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
    /** Dummy components to be used in tests. */
    std::vector<circuit::Component> mDummyComponents;
    /** Dummy connections to be used in tests. */
    std::vector<circuit::Connection> mDummyConnections;
    /** Dimension for width and height for boxes of components to be used in tests. */
    const int cDimension{10};
};

/**
 * @brief Tests that a single component is detected.
 */
TEST_F(ComponentSegmentationTest, detectsSingleComponent)
{
    constexpr auto expectedComponents{1};

    // Setup expectations and behavior
    setupDetectComponents(expectedComponents);

    // Detect components
    ImageMat image{};
    ASSERT_TRUE(mComponentSegmentation->detectComponents(image, image, false));

    // Number of components detected
    const auto componentsDetected{mComponentSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that multiple components are detected.
 */
TEST_F(ComponentSegmentationTest, detectsMultipleComponents)
{
    constexpr auto expectedComponents{2};

    // Setup expectations and behavior
    setupDetectComponents(expectedComponents);

    // Detect components
    ImageMat image{};
    ASSERT_TRUE(mComponentSegmentation->detectComponents(image, image, false));

    // Number of components detected
    const auto componentsDetected{mComponentSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components are detected when there are no components contours found.
 */
TEST_F(ComponentSegmentationTest, detectsNoComponentsWhenNoContours)
{
    constexpr auto expectedComponents{0};

    // Setup expectations and behavior
    setupDetectComponents(expectedComponents);

    // Detect components
    ImageMat image{};
    ASSERT_FALSE(mComponentSegmentation->detectComponents(image, image, false));

    // Number of components detected
    const auto componentsDetected{mComponentSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components are detected when the area of the component blob is small.
 */
TEST_F(ComponentSegmentationTest, detectsNoComponentsWhenSmallArea)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Area is smaller than the minimum to consider blob as a component
    constexpr auto contArea{schematicSegmentation::ComponentSegmentation::cBoundingBoxMinArea - 1};
    constexpr auto componentsContours{2};

    // Setup expectations and behavior
    expectMorphOperations();
    onFindContours(componentsContours);
    EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(componentsContours).WillRepeatedly(Return(contArea));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(0);

    // Detect components
    ImageMat image{};
    ASSERT_FALSE(mComponentSegmentation->detectComponents(image, image, false));

    // Number of components detected
    constexpr auto expectedComponents{0};
    const auto componentsDetected{mComponentSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that the images of components are saved during processing, when components are detected.
 */
TEST_F(ComponentSegmentationTest, savesImagesWhenDetectsComponents)
{
    constexpr auto expectedComponents{1};
    constexpr auto saveImages{true};
    const ImageMat image{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(2 + expectedComponents);
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(expectedComponents);
    setupDetectComponents(expectedComponents);

    // Detect components
    ImageMat img{};
    ASSERT_TRUE(mComponentSegmentation->detectComponents(img, img, saveImages));
}

/**
 * @brief Tests that the images of components are not saved during processing, when components are not detected.
 */
TEST_F(ComponentSegmentationTest, savesNoImagesWhenNoDetectedComponents)
{
    constexpr auto expectedComponents{0};
    constexpr auto saveImages{true};
    const ImageMat image{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(2);
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(0);
    setupDetectComponents(expectedComponents);

    // Detect components
    ImageMat img{};
    ASSERT_FALSE(mComponentSegmentation->detectComponents(img, img, saveImages));
}

/**
 * @brief Tests that a component connection is detected when there is a component and a connection with intersection
 * points.
 */
TEST_F(ComponentSegmentationTest, detectsSingleComponentConnection)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(1);

    /*
     * Prepare circuit:
     * - 1 component
     * - 1 connection connected to component
     */
    constexpr auto xCoordBox{0};
    constexpr auto yCoordBox{0};
    setupDummyComponent(xCoordBox, yCoordBox);
    setupDummyConnection(xCoordBox + cDimension + 1, yCoordBox + cDimension + 1);
    mComponentSegmentation->setDetectedComponents(mDummyComponents);

    // Detect component connections
    ImageMat img{};
    mComponentSegmentation->detectComponentConnections(img, mDummyConnections);

    // Check component connections
    const auto detectedComponents{mComponentSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 1);
    EXPECT_EQ(detectedComponents.at(0).mConnections.size(), 1);
}

/**
 * @brief Tests that multiple component connections are detected when there are components and connections with
 * intersection points.
 */
TEST_F(ComponentSegmentationTest, detectsMultipleComponentConnection)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(4);

    /*
     * Prepare circuit:
     * - 2 components, A and B
     * - 2 connections:
     *      - 1 connection connected to component A and to component B
     *      - 1 connection connected to component B only
     */
    constexpr auto xCoordBox1{0};
    constexpr auto yCoordBox1{0};
    constexpr auto xCoordBox2{12};
    constexpr auto yCoordBox2{12};
    setupDummyComponent(xCoordBox1, yCoordBox1);
    setupDummyComponent(xCoordBox2, yCoordBox2);
    setupDummyConnection(xCoordBox1 + cDimension + 1, yCoordBox1 + cDimension + 1);
    setupDummyConnection(xCoordBox2 + cDimension, yCoordBox2 + cDimension);
    mComponentSegmentation->setDetectedComponents(mDummyComponents);

    // Detect component connections
    ImageMat img{};
    mComponentSegmentation->detectComponentConnections(img, mDummyConnections);

    // Check component connections
    const auto detectedComponents{mComponentSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 2);
    EXPECT_EQ(detectedComponents.at(0).mConnections.size(), 1);
    EXPECT_EQ(detectedComponents.at(1).mConnections.size(), 2);
}

/**
 * @brief Tests that a component connection is not detected when there is a component and a connection without
 * intersection points.
 */
TEST_F(ComponentSegmentationTest, detectsNoComponentConnectionPoints)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(1);

    /*
     * Prepare circuit:
     * - 1 component
     * - 1 connection not connected to component
     */
    constexpr auto xCoordBox{0};
    constexpr auto yCoordBox{0};
    setupDummyComponent(xCoordBox, yCoordBox);
    setupDummyConnection(xCoordBox + cDimension + 2, yCoordBox + cDimension + 2);
    mComponentSegmentation->setDetectedComponents(mDummyComponents);

    // Detect component connections
    ImageMat img{};
    mComponentSegmentation->detectComponentConnections(img, mDummyConnections);

    // Check component connections
    const auto detectedComponents{mComponentSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 1);
    EXPECT_EQ(detectedComponents.at(0).mConnections.size(), 0);
}

/**
 * @brief Tests that a component connection is not detected when there is a component but no connections detected.
 */
TEST_F(ComponentSegmentationTest, detectsNoComponentConnectionNoConnections)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(0);

    /*
     * Prepare circuit:
     * - 1 component
     * - 0 connections
     */
    constexpr auto xCoordBox{0};
    constexpr auto yCoordBox{0};
    setupDummyComponent(xCoordBox, yCoordBox);
    mComponentSegmentation->setDetectedComponents(mDummyComponents);

    // Detect component connections
    ImageMat img{};
    mComponentSegmentation->detectComponentConnections(img, {});

    // Check component connections
    const auto detectedComponents{mComponentSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 1);
    EXPECT_EQ(detectedComponents.at(0).mConnections.size(), 0);
}

/**
 * @brief Tests that a component connection is not detected when there are no components and no connections detected.
 */
TEST_F(ComponentSegmentationTest, detectsNoComponentConnectionNoComponents)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    onCheckContainsPoint();
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(0);

    /*
     * Prepare circuit:
     * - 0 components
     * - 0 connections
     */
    mComponentSegmentation->setDetectedComponents({});

    // Detect component connections
    ImageMat img{};
    mComponentSegmentation->detectComponentConnections(img, {});

    // Check component connections
    const auto detectedComponents{mComponentSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 0);
}

/**
 * @brief Tests that the list of detected components is kept when all the components have connections connected.
 */
TEST_F(ComponentSegmentationTest, updatesDetectedComponentsKept)
{
    /*
     * Prepare circuit:
     * - 2 components, A and B
     * - 1 connection connected to component A and to component B
     */
    constexpr auto xCoordBox1{0};
    constexpr auto yCoordBox1{0};
    constexpr auto xCoordBox2{12};
    constexpr auto yCoordBox2{12};
    setupDummyComponent(xCoordBox1, yCoordBox1);
    setupDummyComponent(xCoordBox2, yCoordBox2);
    setupDummyConnection(xCoordBox1 + cDimension + 1, yCoordBox1 + cDimension + 1);
    mDummyComponents.at(0).mConnections = mDummyConnections;
    mDummyComponents.at(1).mConnections = mDummyConnections;
    mComponentSegmentation->setDetectedComponents(mDummyComponents);

    // Update detected components
    ASSERT_TRUE(mComponentSegmentation->updateDetectedComponents());

    // Check detected components
    const auto detectedComponents{mComponentSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 2);
}

/**
 * @brief Tests that the list of detected components is updated when there are components with no connections
 connected.
 */
TEST_F(ComponentSegmentationTest, updatesDetectedComponents)
{
    /*
     * Prepare circuit:
     * - 2 components, A and B
     * - 1 connection connected to component A
     * - 0 connections connected to component B
     */
    constexpr auto xCoordBox1{0};
    constexpr auto yCoordBox1{0};
    constexpr auto xCoordBox2{20};
    constexpr auto yCoordBox2{20};
    setupDummyComponent(xCoordBox1, yCoordBox1);
    setupDummyComponent(xCoordBox2, yCoordBox2);
    setupDummyConnection(xCoordBox1 + cDimension + 1, yCoordBox1 + cDimension + 1);
    mDummyComponents.at(0).mConnections = mDummyConnections;
    mDummyComponents.at(1).mConnections = {};
    mComponentSegmentation->setDetectedComponents(mDummyComponents);

    // Update detected components
    ASSERT_TRUE(mComponentSegmentation->updateDetectedComponents());

    // Check detected components
    const auto detectedComponents{mComponentSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 1);
}

/**
 * @brief Tests that the list of detected components becomes empty when all components with no connections connected.
 */
TEST_F(ComponentSegmentationTest, updatesDetectedComponentsEmpty)
{
    /*
     * Prepare circuit:
     * - 2 components, A and B
     * - 0 connections connected to component A
     * - 0 connections connected to component B
     */
    constexpr auto xCoordBox1{0};
    constexpr auto yCoordBox1{0};
    constexpr auto xCoordBox2{20};
    constexpr auto yCoordBox2{20};
    setupDummyComponent(xCoordBox1, yCoordBox1);
    setupDummyComponent(xCoordBox2, yCoordBox2);
    mDummyComponents.at(0).mConnections = {};
    mDummyComponents.at(1).mConnections = {};
    mComponentSegmentation->setDetectedComponents(mDummyComponents);

    // Update detected components
    ASSERT_FALSE(mComponentSegmentation->updateDetectedComponents());

    // Check detected components
    const auto detectedComponents{mComponentSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 0);
}

/**
 * @brief Tests that component bounding box is built when the area of the component blob is large.
 */
TEST_F(ComponentSegmentationTest, boundingBoxSingleComponent)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Area is equal to the minimum to consider blob as a component
    constexpr auto contArea{schematicSegmentation::ComponentSegmentation::cBoundingBoxMinArea};
    const Contours componentsContours{Contour{}};
    const Rectangle rect{0, 0, 100, 100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(1).WillOnce(Return(contArea));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(1).WillOnce(Return(rect));

    // Bounding boxes
    ImageMat img{};
    mComponentSegmentation->boundingBoxComponents(componentsContours, img);

    // Number of components detected
    constexpr auto expectedComponents{1};
    const auto componentsDetected{mComponentSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that components bounding boxes are built when the area of the components blobs are large.
 */
TEST_F(ComponentSegmentationTest, boundingBoxMultipleComponents)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Area is equal to the minimum to consider blob as a component
    constexpr auto contArea{schematicSegmentation::ComponentSegmentation::cBoundingBoxMinArea};
    const Contours componentsContours{Contour{}, Contour{}};
    const Rectangle rect{0, 0, 100, 100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(2).WillRepeatedly(Return(contArea));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(2).WillRepeatedly(Return(rect));

    // Bounding boxes
    ImageMat img{};
    mComponentSegmentation->boundingBoxComponents(componentsContours, img);

    // Number of components detected
    constexpr auto expectedComponents{2};
    const auto componentsDetected{mComponentSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components bounding boxes are built when there are no components blobs.
 */
TEST_F(ComponentSegmentationTest, noBoundingBoxComponentWhenNoBlobs)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Empty components blobs
    const Contours componentsBlobs{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(0);

    // Bounding boxes
    ImageMat img{};
    mComponentSegmentation->boundingBoxComponents(componentsBlobs, img);

    // Number of components detected
    constexpr auto expectedComponents{0};
    const auto componentsDetected{mComponentSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components bounding boxes are built when the area of the component blob is small.
 */
TEST_F(ComponentSegmentationTest, noBoundingBoxComponentWhenSmallArea)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Area is smaller than the minimum to consider blob as a component
    constexpr auto contArea{schematicSegmentation::ComponentSegmentation::cBoundingBoxMinArea - 1};
    const Contours componentsBlobs{Contour{}};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(1).WillOnce(Return(contArea));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(0);

    // Bounding boxes
    ImageMat img{};
    mComponentSegmentation->boundingBoxComponents(componentsBlobs, img);

    // Number of components detected
    constexpr auto expectedComponents{0};
    const auto componentsDetected{mComponentSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that the component bounding box size increases and is centered.
 */
TEST_F(ComponentSegmentationTest, increasesBoundingBoxComponent)
{
    const Rectangle box{20, 20, 50, 50};
    constexpr auto widthIncr{20};
    constexpr auto heightIncr{20};
    constexpr auto widthMax{100};
    constexpr auto heightMax{100};

    // Increase bounding box
    const auto increasedBox{
        mComponentSegmentation->increaseBoundingBox(box, widthIncr, heightIncr, widthMax, heightMax)};

    // Expectations
    const auto expectedX{box.x - widthIncr / 2};
    const auto expectedY{box.y - heightIncr / 2};
    const auto expectedWidth{box.width + widthIncr};
    const auto expectedHeight{box.height + heightIncr};
    EXPECT_EQ(increasedBox.x, expectedX);
    EXPECT_EQ(increasedBox.y, expectedY);
    EXPECT_EQ(increasedBox.width, expectedWidth);
    EXPECT_EQ(increasedBox.height, expectedHeight);
}

/**
 * @brief Tests that the component bounding box size increases for the case when it is on top left corner.
 */
TEST_F(ComponentSegmentationTest, increasesBoundingBoxComponentTopLeftCorner)
{
    const Rectangle box{0, 0, 50, 50};
    constexpr auto widthIncr{20};
    constexpr auto heightIncr{20};
    constexpr auto widthMax{100};
    constexpr auto heightMax{100};

    // Increase bounding box
    const auto increasedBox{
        mComponentSegmentation->increaseBoundingBox(box, widthIncr, heightIncr, widthMax, heightMax)};

    // Expectations
    const auto expectedX{0};
    const auto expectedY{0};
    const auto expectedWidth{box.width + widthIncr};
    const auto expectedHeight{box.height + heightIncr};
    EXPECT_EQ(increasedBox.x, expectedX);
    EXPECT_EQ(increasedBox.y, expectedY);
    EXPECT_EQ(increasedBox.width, expectedWidth);
    EXPECT_EQ(increasedBox.height, expectedHeight);
}

/**
 * @brief Tests that the component bounding box size increases for the case when it is on bottom right corner.
 */
TEST_F(ComponentSegmentationTest, increasesBoundingBoxComponentBottomRightCorner)
{
    const Rectangle box{90, 90, 10, 10};
    constexpr auto widthIncr{20};
    constexpr auto heightIncr{20};
    constexpr auto widthMax{100};
    constexpr auto heightMax{100};

    // Increase bounding box
    const auto increasedBox{
        mComponentSegmentation->increaseBoundingBox(box, widthIncr, heightIncr, widthMax, heightMax)};

    // Expectations
    const auto expectedX{box.x - widthIncr / 2};
    const auto expectedY{box.y - heightIncr / 2};
    const auto expectedWidth{widthMax - expectedX};
    const auto expectedHeight{heightMax - expectedY};
    EXPECT_EQ(increasedBox.x, expectedX);
    EXPECT_EQ(increasedBox.y, expectedY);
    EXPECT_EQ(increasedBox.width, expectedWidth);
    EXPECT_EQ(increasedBox.height, expectedHeight);
}

// /**
//  * @brief Main function.
//  *
//  * @param argc Number of command line arguments.
//  * @param argv Command line arguments.
//  *
//  * @return 0 on success, 1 on failure.
//  */
// int main(int argc, char* argv[])
// {
//     testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
