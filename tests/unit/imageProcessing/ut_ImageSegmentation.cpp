/**
 * @file
 */

#include "imageProcessing/ImageSegmentation.h"
#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace circuitSegmentation;
using namespace circuitSegmentation::computerVision;

/**
 * @brief Test class of ImageSegmentation.
 */
class ImageSegmentationTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockOpenCvWrapper = std::make_shared<NiceMock<MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);

        mImageSegmentation = std::make_unique<imageProcessing::ImageSegmentation>(mMockOpenCvWrapper, mLogger, true);
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
        const auto contArea{componentsDetected > 0 ? imageProcessing::ImageSegmentation::cBoundingBoxMinArea
                                                   : (imageProcessing::ImageSegmentation::cBoundingBoxMinArea - 1)};
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
     * @brief Expects calls to remove components when detecting nodes.
     */
    void expectRemoveComponents()
    {
        const ImageMat image{};
        const std::vector<circuit::Component> components{circuit::Component{}};

        // Set components
        mImageSegmentation->setDetectedComponents(components);

        // Setup expectations
        EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).WillRepeatedly(Return(image));
        EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(1);
    }

    /**
     * @brief Sets up detection of nodes.
     *
     * @param nodesDetected Number of nodes detected.
     */
    void setupDetectNodes(const unsigned int nodesDetected)
    {
        // Area is larger than the minimum to consider wire as a node, if there are nodes to detect
        const auto contArea{nodesDetected > 0 ? imageProcessing::ImageSegmentation::cNodesMinArea
                                              : (imageProcessing::ImageSegmentation::cNodesMinArea - 1)};

        // Setup expectations and behavior
        expectRemoveComponents();
        onFindContours(nodesDetected);

        EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
        EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(nodesDetected).WillRepeatedly(Return(contArea));
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
        dummyComponent.setBoundingBox(rectangle);

        // Set
        mDummyComponents.push_back(dummyComponent);
    }

    /**
     * @brief Sets up a dummy node.
     *
     * @param x X coordinate for a point of node wire.
     * @param y Y coordinate for a point of node wire.
     */
    void setupDummyNode(const int& x, const int& y)
    {
        // Wire
        const circuit::Wire wire{{x, y}};

        // Dummy node
        circuit::Node dummyNode{};
        dummyNode.setWire(wire);

        // Set
        mDummyNodes.push_back(dummyNode);
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
    /** Image segmentation. */
    std::unique_ptr<imageProcessing::ImageSegmentation> mImageSegmentation;
    /** OpenCV wrapper. */
    std::shared_ptr<NiceMock<MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
    /** Dummy components to be used in tests. */
    std::vector<circuit::Component> mDummyComponents;
    /** Dummy nodes to be used in tests. */
    std::vector<circuit::Node> mDummyNodes;
    /** Dimension for width and height for boxes of components to be used in tests. */
    const int cDimension{10};
};

/**
 * @brief Tests that a single component is detected.
 */
TEST_F(ImageSegmentationTest, detectsSingleComponent)
{
    constexpr auto expectedComponents{1};

    // Setup expectations and behavior
    setupDetectComponents(expectedComponents);

    // Detect components
    ASSERT_TRUE(mImageSegmentation->detectComponents());

    // Number of components detected
    const auto componentsDetected{mImageSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that multiple components are detected.
 */
TEST_F(ImageSegmentationTest, detectsMultipleComponents)
{
    constexpr auto expectedComponents{2};

    // Setup expectations and behavior
    setupDetectComponents(expectedComponents);

    // Detect components
    ASSERT_TRUE(mImageSegmentation->detectComponents());

    // Number of components detected
    const auto componentsDetected{mImageSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components are detected when there are no components contours found.
 */
TEST_F(ImageSegmentationTest, detectsNoComponentsWhenNoContours)
{
    constexpr auto expectedComponents{0};

    // Setup expectations and behavior
    setupDetectComponents(expectedComponents);

    // Detect components
    ASSERT_FALSE(mImageSegmentation->detectComponents());

    // Number of components detected
    const auto componentsDetected{mImageSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components are detected when the area of the component blob is small.
 */
TEST_F(ImageSegmentationTest, detectsNoComponentsWhenSmallArea)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Area is smaller than the minimum to consider blob as a component
    constexpr auto contArea{imageProcessing::ImageSegmentation::cBoundingBoxMinArea - 1};
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
    ASSERT_FALSE(mImageSegmentation->detectComponents());

    // Number of components detected
    constexpr auto expectedComponents{0};
    const auto componentsDetected{mImageSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that the images of components are saved during processing, when components are detected.
 */
TEST_F(ImageSegmentationTest, savesImagesWhenDetectsComponents)
{
    constexpr auto expectedComponents{1};
    constexpr auto saveImages{true};
    const ImageMat image{};

    // Set save images
    mImageSegmentation->setSaveImages(saveImages);

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(2 + expectedComponents);
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(expectedComponents);
    setupDetectComponents(expectedComponents);

    // Detect components
    ASSERT_TRUE(mImageSegmentation->detectComponents());
}

/**
 * @brief Tests that the images of components are not saved during processing, when components are not detected.
 */
TEST_F(ImageSegmentationTest, savesNoImagesWhenNoDetectedComponents)
{
    constexpr auto expectedComponents{0};
    constexpr auto saveImages{true};
    const ImageMat image{};

    // Set save images
    mImageSegmentation->setSaveImages(saveImages);

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(2);
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(0);
    setupDetectComponents(expectedComponents);

    // Detect components
    ASSERT_FALSE(mImageSegmentation->detectComponents());
}

/**
 * @brief Tests that component bounding box is built when the area of the component blob is large.
 */
TEST_F(ImageSegmentationTest, boundingBoxSingleComponent)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Area is equal to the minimum to consider blob as a component
    constexpr auto contArea{imageProcessing::ImageSegmentation::cBoundingBoxMinArea};
    const Contours componentsContours{Contour{}};
    const Rectangle rect{0, 0, 100, 100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(1).WillOnce(Return(contArea));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(1).WillOnce(Return(rect));

    // Bounding boxes
    mImageSegmentation->boundingBoxComponents(componentsContours);

    // Number of components detected
    constexpr auto expectedComponents{1};
    const auto componentsDetected{mImageSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that components bounding boxes are built when the area of the components blobs are large.
 */
TEST_F(ImageSegmentationTest, boundingBoxMultipleComponents)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Area is equal to the minimum to consider blob as a component
    constexpr auto contArea{imageProcessing::ImageSegmentation::cBoundingBoxMinArea};
    const Contours componentsContours{Contour{}, Contour{}};
    const Rectangle rect{0, 0, 100, 100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(2).WillRepeatedly(Return(contArea));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(2).WillRepeatedly(Return(rect));

    // Bounding boxes
    mImageSegmentation->boundingBoxComponents(componentsContours);

    // Number of components detected
    constexpr auto expectedComponents{2};
    const auto componentsDetected{mImageSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components bounding boxes are built when there are no components blobs.
 */
TEST_F(ImageSegmentationTest, noBoundingBoxComponentWhenNoBlobs)
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
    mImageSegmentation->boundingBoxComponents(componentsBlobs);

    // Number of components detected
    constexpr auto expectedComponents{0};
    const auto componentsDetected{mImageSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components bounding boxes are built when the area of the component blob is small.
 */
TEST_F(ImageSegmentationTest, noBoundingBoxComponentWhenSmallArea)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Area is smaller than the minimum to consider blob as a component
    constexpr auto contArea{imageProcessing::ImageSegmentation::cBoundingBoxMinArea - 1};
    const Contours componentsBlobs{Contour{}};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(1).WillOnce(Return(contArea));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(0);

    // Bounding boxes
    mImageSegmentation->boundingBoxComponents(componentsBlobs);

    // Number of components detected
    constexpr auto expectedComponents{0};
    const auto componentsDetected{mImageSegmentation->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that the component bounding box size increases and is centered.
 */
TEST_F(ImageSegmentationTest, increasesBoundingBoxComponent)
{
    const Rectangle box{20, 20, 50, 50};
    constexpr auto widthIncr{20};
    constexpr auto heightIncr{20};
    constexpr auto widthMax{100};
    constexpr auto heightMax{100};

    // Increase bounding box
    const auto increasedBox{mImageSegmentation->increaseBoundingBox(box, widthIncr, heightIncr, widthMax, heightMax)};

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
TEST_F(ImageSegmentationTest, increasesBoundingBoxComponentTopLeftCorner)
{
    const Rectangle box{0, 0, 50, 50};
    constexpr auto widthIncr{20};
    constexpr auto heightIncr{20};
    constexpr auto widthMax{100};
    constexpr auto heightMax{100};

    // Increase bounding box
    const auto increasedBox{mImageSegmentation->increaseBoundingBox(box, widthIncr, heightIncr, widthMax, heightMax)};

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
TEST_F(ImageSegmentationTest, increasesBoundingBoxComponentBottomRightCorner)
{
    const Rectangle box{90, 90, 10, 10};
    constexpr auto widthIncr{20};
    constexpr auto heightIncr{20};
    constexpr auto widthMax{100};
    constexpr auto heightMax{100};

    // Increase bounding box
    const auto increasedBox{mImageSegmentation->increaseBoundingBox(box, widthIncr, heightIncr, widthMax, heightMax)};

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

/**
 * @brief Tests that a single node is detected.
 */
TEST_F(ImageSegmentationTest, detectsSingleNode)
{
    constexpr auto expectedNodes{1};

    // Setup expectations and behavior
    setupDetectNodes(expectedNodes);

    // Detect nodes
    ASSERT_TRUE(mImageSegmentation->detectNodes());

    // Number of nodes detected
    const auto nodesDetected{mImageSegmentation->getDetectedNodes().size()};
    EXPECT_EQ(nodesDetected, expectedNodes);
}

/**
 * @brief Tests that multiple nodes are detected.
 */
TEST_F(ImageSegmentationTest, detectsMultipleNodes)
{
    constexpr auto expectedNodes{2};

    // Setup expectations and behavior
    setupDetectNodes(expectedNodes);

    // Detect nodes
    ASSERT_TRUE(mImageSegmentation->detectNodes());

    // Number of nodes detected
    const auto nodesDetected{mImageSegmentation->getDetectedNodes().size()};
    EXPECT_EQ(nodesDetected, expectedNodes);
}

/**
 * @brief Tests that no nodes are detected when there are no nodes contours found.
 */
TEST_F(ImageSegmentationTest, detectsNoNodesWhenNoContours)
{
    constexpr auto expectedNodes{0};

    // Setup expectations and behavior
    setupDetectNodes(expectedNodes);

    // Detect nodes
    ASSERT_FALSE(mImageSegmentation->detectNodes());

    // Number of nodes detected
    const auto nodesDetected{mImageSegmentation->getDetectedNodes().size()};
    EXPECT_EQ(nodesDetected, expectedNodes);
}

/**
 * @brief Tests that no nodes are detected when the area of the node wire is small.
 */
TEST_F(ImageSegmentationTest, detectsNoNodesWhenSmallArea)
{
    // Area is smaller than the minimum to consider wire as a node
    constexpr auto contArea{imageProcessing::ImageSegmentation::cNodesMinArea - 1};
    constexpr auto nodesContours{2};

    // Setup expectations and behavior
    expectRemoveComponents();
    onFindContours(nodesContours);
    EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(nodesContours).WillRepeatedly(Return(contArea));

    // Detect nodes
    ASSERT_FALSE(mImageSegmentation->detectNodes());

    // Number of nodes detected
    constexpr auto expectedNodes{0};
    const auto nodesDetected{mImageSegmentation->getDetectedNodes().size()};
    EXPECT_EQ(nodesDetected, expectedNodes);
}

/**
 * @brief Tests that the image with detected nodes is saved during processing, when nodes are detected.
 */
TEST_F(ImageSegmentationTest, savesImagesWhenDetectsNodes)
{
    constexpr auto expectedNodes{1};
    constexpr auto saveImages{true};

    // Set save images
    mImageSegmentation->setSaveImages(saveImages);

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(2);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(1);
    setupDetectNodes(expectedNodes);

    // Detect nodes
    ASSERT_TRUE(mImageSegmentation->detectNodes());
}

/**
 * @brief Tests that the image with detected nodes is not saved during processing, when nodes are not detected.
 */
TEST_F(ImageSegmentationTest, savesNoImagesWhenNoDetectedNodes)
{
    constexpr auto expectedNodes{0};
    constexpr auto saveImages{true};
    const ImageMat image{};

    // Set save images
    mImageSegmentation->setSaveImages(saveImages);

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(0);
    setupDetectNodes(expectedNodes);

    // Detect nodes
    ASSERT_FALSE(mImageSegmentation->detectNodes());
}

/**
 * @brief Tests that a circuit connection is detected when there is a component and a node with intersection points.
 */
TEST_F(ImageSegmentationTest, detectsSingleCircuitConnection)
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
     * - 1 node connected to component
     */
    constexpr auto xCoordBox{0};
    constexpr auto yCoordBox{0};
    setupDummyComponent(xCoordBox, yCoordBox);
    setupDummyNode(xCoordBox + cDimension + 1, yCoordBox + cDimension + 1);
    mImageSegmentation->setDetectedComponents(mDummyComponents);
    mImageSegmentation->setDetectedNodes(mDummyNodes);

    // Detect circuit connections
    mImageSegmentation->detectCircuitConnections();

    // Check circuit connections
    const auto detectedComponents{mImageSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 1);
    EXPECT_EQ(detectedComponents.at(0).getNodes().size(), 1);
}

/**
 * @brief Tests that multiple circuit connections are detected when there are components and nodes with intersection
 * points.
 */
TEST_F(ImageSegmentationTest, detectsMultipleCircuitConnection)
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
     * - 2 nodes:
     *      - 1 node connected to component A and to component B
     *      - 1 node connected to component B only
     */
    constexpr auto xCoordBox1{0};
    constexpr auto yCoordBox1{0};
    constexpr auto xCoordBox2{12};
    constexpr auto yCoordBox2{12};
    setupDummyComponent(xCoordBox1, yCoordBox1);
    setupDummyComponent(xCoordBox2, yCoordBox2);
    setupDummyNode(xCoordBox1 + cDimension + 1, yCoordBox1 + cDimension + 1);
    setupDummyNode(xCoordBox2 + cDimension, yCoordBox2 + cDimension);
    mImageSegmentation->setDetectedComponents(mDummyComponents);
    mImageSegmentation->setDetectedNodes(mDummyNodes);

    // Detect circuit connections
    mImageSegmentation->detectCircuitConnections();

    // Check circuit connections
    const auto detectedComponents{mImageSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 2);
    EXPECT_EQ(detectedComponents.at(0).getNodes().size(), 1);
    EXPECT_EQ(detectedComponents.at(1).getNodes().size(), 2);
}

/**
 * @brief Tests that a circuit connection is not detected when there is a component and a node without intersection
 * points.
 */
TEST_F(ImageSegmentationTest, detectsNoCircuitConnectionPoints)
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
     * - 1 node not connected to component
     */
    constexpr auto xCoordBox{0};
    constexpr auto yCoordBox{0};
    setupDummyComponent(xCoordBox, yCoordBox);
    setupDummyNode(xCoordBox + cDimension + 2, yCoordBox + cDimension + 2);
    mImageSegmentation->setDetectedComponents(mDummyComponents);
    mImageSegmentation->setDetectedNodes(mDummyNodes);

    // Detect circuit connections
    mImageSegmentation->detectCircuitConnections();

    // Check circuit connections
    const auto detectedComponents{mImageSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 1);
    EXPECT_EQ(detectedComponents.at(0).getNodes().size(), 0);
}

/**
 * @brief Tests that a circuit connection is not detected when there is a component but no nodes detected.
 */
TEST_F(ImageSegmentationTest, detectsNoCircuitConnectionNoNodes)
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
     * - 0 nodes
     */
    constexpr auto xCoordBox{0};
    constexpr auto yCoordBox{0};
    setupDummyComponent(xCoordBox, yCoordBox);
    mImageSegmentation->setDetectedComponents(mDummyComponents);
    mImageSegmentation->setDetectedNodes({});

    // Detect circuit connections
    mImageSegmentation->detectCircuitConnections();

    // Check circuit connections
    const auto detectedComponents{mImageSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 1);
    EXPECT_EQ(detectedComponents.at(0).getNodes().size(), 0);
}

/**
 * @brief Tests that a circuit connection is not detected when there are no components and no nodes detected.
 */
TEST_F(ImageSegmentationTest, detectsNoCircuitConnectionNoComponents)
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
     * - 0 nodes
     */
    mImageSegmentation->setDetectedComponents({});
    mImageSegmentation->setDetectedNodes({});

    // Detect circuit connections
    mImageSegmentation->detectCircuitConnections();

    // Check circuit connections
    const auto detectedComponents{mImageSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 0);
}

/**
 * @brief Tests that the list of detected components is kept when all the components have nodes connected.
 */
TEST_F(ImageSegmentationTest, updatesDetectedComponentsKept)
{
    /*
     * Prepare circuit:
     * - 2 components, A and B
     * - 1 node connected to component A and to component B
     */
    constexpr auto xCoordBox1{0};
    constexpr auto yCoordBox1{0};
    constexpr auto xCoordBox2{12};
    constexpr auto yCoordBox2{12};
    setupDummyComponent(xCoordBox1, yCoordBox1);
    setupDummyComponent(xCoordBox2, yCoordBox2);
    setupDummyNode(xCoordBox1 + cDimension + 1, yCoordBox1 + cDimension + 1);
    mDummyComponents.at(0).setNodes(mDummyNodes);
    mDummyComponents.at(1).setNodes(mDummyNodes);
    mImageSegmentation->setDetectedComponents(mDummyComponents);

    // Update detected components
    ASSERT_TRUE(mImageSegmentation->updateDetectedComponents());

    // Check detected components
    const auto detectedComponents{mImageSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 2);
}

/**
 * @brief Tests that the list of detected components is updated when there are components with no nodes connected.
 */
TEST_F(ImageSegmentationTest, updatesDetectedComponents)
{
    /*
     * Prepare circuit:
     * - 2 components, A and B
     * - 1 node connected to component A
     * - 0 nodes connected to component B
     */
    constexpr auto xCoordBox1{0};
    constexpr auto yCoordBox1{0};
    constexpr auto xCoordBox2{20};
    constexpr auto yCoordBox2{20};
    setupDummyComponent(xCoordBox1, yCoordBox1);
    setupDummyComponent(xCoordBox2, yCoordBox2);
    setupDummyNode(xCoordBox1 + cDimension + 1, yCoordBox1 + cDimension + 1);
    mDummyComponents.at(0).setNodes(mDummyNodes);
    mDummyComponents.at(1).setNodes({});
    mImageSegmentation->setDetectedComponents(mDummyComponents);

    // Update detected components
    ASSERT_TRUE(mImageSegmentation->updateDetectedComponents());

    // Check detected components
    const auto detectedComponents{mImageSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 1);
}

/**
 * @brief Tests that the list of detected components becomes empty when all components with no nodes connected.
 */
TEST_F(ImageSegmentationTest, updatesDetectedComponentsEmpty)
{
    /*
     * Prepare circuit:
     * - 2 components, A and B
     * - 0 nodes connected to component A
     * - 0 nodes connected to component B
     */
    constexpr auto xCoordBox1{0};
    constexpr auto yCoordBox1{0};
    constexpr auto xCoordBox2{20};
    constexpr auto yCoordBox2{20};
    setupDummyComponent(xCoordBox1, yCoordBox1);
    setupDummyComponent(xCoordBox2, yCoordBox2);
    mDummyComponents.at(0).setNodes({});
    mDummyComponents.at(1).setNodes({});
    mImageSegmentation->setDetectedComponents(mDummyComponents);

    // Update detected components
    ASSERT_FALSE(mImageSegmentation->updateDetectedComponents());

    // Check detected components
    const auto detectedComponents{mImageSegmentation->getDetectedComponents()};
    EXPECT_EQ(detectedComponents.size(), 0);
}

/**
 * @brief Tests that the flag to save images is defined correctly.
 */
TEST_F(ImageSegmentationTest, setsSaveImages)
{
    const auto saveImages{true};
    mImageSegmentation->setSaveImages(saveImages);

    EXPECT_EQ(saveImages, mImageSegmentation->getSaveImages());
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
