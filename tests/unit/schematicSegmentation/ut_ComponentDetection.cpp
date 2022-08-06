/**
 * @file
 */

#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include "schematicSegmentation/ComponentDetection.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace circuitSegmentation;
using namespace circuitSegmentation::computerVision;

/**
 * @brief Test class of ComponentDetection.
 */
class ComponentDetectionTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockOpenCvWrapper = std::make_shared<NiceMock<MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);

        mComponentDetection = std::make_unique<schematicSegmentation::ComponentDetection>(mMockOpenCvWrapper, mLogger);

        setupDummyConnection();
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /**
     * @brief Sets the behavior when finding contours.
     *
     * @param numContours Number of contours found.
     */
    void onFindContours(const unsigned int numContours)
    {
        ON_CALL(*mMockOpenCvWrapper, findContours)
            .WillByDefault([numContours]([[maybe_unused]] ImageMat& image,
                                         Contours& contours,
                                         [[maybe_unused]] ContoursHierarchy& hierarchy,
                                         [[maybe_unused]] const OpenCvWrapper::RetrievalModes& mode,
                                         [[maybe_unused]] const OpenCvWrapper::ContourApproximationModes& method) {
                for (unsigned int i{0}; i < numContours; ++i) {
                    Contour contour{};
                    contours.push_back(contour);
                }
            });
    }

    /**
     * @brief Expects call to remove connections from image.
     */
    void expectRemoveConnections()
    {
        // Setup expectations
        EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(1);
    }

    /**
     * @brief Expects calls to morphological operations.
     */
    void expectMorphOperations()
    {
        const ImageMat kernel{};

        // Setup expectations
        EXPECT_CALL(*mMockOpenCvWrapper, getStructuringElement(OpenCvWrapper::MorphShapes::MORPH_RECT, _))
            .Times(1)
            .WillRepeatedly(Return(kernel));
        EXPECT_CALL(*mMockOpenCvWrapper, morphologyEx(_, _, OpenCvWrapper::MorphTypes::MORPH_CLOSE, _, _)).Times(1);
    }

    /**
     * @brief Sets the behavior when checking contour.
     *
     * @param numComponentsDetected Number of components detected.
     */
    void onCheckContour(const unsigned int numComponentsDetected)
    {
        constexpr auto imgWidth{100};
        constexpr auto imgHeight{100};
        const Rectangle rect{0, 0, 10, 10};
        // Area is larger than the minimum to consider rectangle as a component, if there are components to detect
        const auto rectArea{numComponentsDetected > 0 ? schematicSegmentation::ComponentDetection::cBoxMinArea
                                                      : (schematicSegmentation::ComponentDetection::cBoxMinArea - 1)};

        // Setup expectations and behavior
        ON_CALL(*mMockOpenCvWrapper, getImageWidth).WillByDefault(Return(imgWidth));
        ON_CALL(*mMockOpenCvWrapper, getImageHeight).WillByDefault(Return(imgHeight));
        ON_CALL(*mMockOpenCvWrapper, boundingRect).WillByDefault(Return(rect));
        ON_CALL(*mMockOpenCvWrapper, rectangleArea).WillByDefault(Return(rectArea));
        ON_CALL(*mMockOpenCvWrapper, contains).WillByDefault(Return(true));
        EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(numComponentsDetected);
        EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(numComponentsDetected);
        EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(numComponentsDetected);
        EXPECT_CALL(*mMockOpenCvWrapper, rectangleArea).Times(numComponentsDetected);
        EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(numComponentsDetected);
    }

    /**
     * @brief Sets up detection of components.
     *
     * @param numComponentsDetected Number of components detected.
     */
    void setupDetectComponents(const unsigned int numComponentsDetected)
    {
        // Setup expectations and behavior
        expectRemoveConnections();
        expectMorphOperations();
        onFindContours(numComponentsDetected);
        onCheckContour(numComponentsDetected);

        EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
    }

    /**
     * @brief Sets up a dummy connection.
     */
    void setupDummyConnection()
    {
        // Wire
        constexpr auto x{5};
        constexpr auto y{5};
        const circuit::Wire wire{{x, y}};

        // Dummy connection
        circuit::Connection dummyConnection{};
        dummyConnection.mWire = wire;

        // Set
        mDummyConnections.push_back(dummyConnection);
    }

protected:
    /** Component detection. */
    std::unique_ptr<schematicSegmentation::ComponentDetection> mComponentDetection;
    /** OpenCV wrapper. */
    std::shared_ptr<NiceMock<MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
    /** Dummy connections to be used in tests. */
    std::vector<circuit::Connection> mDummyConnections;
};

/**
 * @brief Tests that a single component is detected.
 */
TEST_F(ComponentDetectionTest, detectsSingleComponent)
{
    constexpr auto expectedComponents{1};

    // Setup expectations and behavior
    setupDetectComponents(expectedComponents);

    // Detect components
    ImageMat image{};
    ASSERT_TRUE(mComponentDetection->detectComponents(image, image, mDummyConnections, false));

    // Number of components detected
    const auto componentsDetected{mComponentDetection->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that multiple components are detected.
 */
TEST_F(ComponentDetectionTest, detectsMultipleComponents)
{
    constexpr auto expectedComponents{2};

    // Setup expectations and behavior
    setupDetectComponents(expectedComponents);

    // Detect components
    ImageMat image{};
    ASSERT_TRUE(mComponentDetection->detectComponents(image, image, mDummyConnections, false));

    // Number of components detected
    const auto componentsDetected{mComponentDetection->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components are detected when there are no components contours found.
 */
TEST_F(ComponentDetectionTest, detectsNoComponentsWhenNoContours)
{
    constexpr auto expectedComponents{0};

    // Setup expectations and behavior
    setupDetectComponents(expectedComponents);

    // Detect components
    ImageMat image{};
    ASSERT_FALSE(mComponentDetection->detectComponents(image, image, mDummyConnections, false));

    // Number of components detected
    const auto componentsDetected{mComponentDetection->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components are detected when the area of the component is small.
 */
TEST_F(ComponentDetectionTest, detectsNoComponentsWhenSmallArea)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    const Rectangle rect{0, 0, 10, 10};
    // Area is smaller than the minimum to consider rectangle as a component
    const auto rectArea{schematicSegmentation::ComponentDetection::cBoxMinArea - 1};

    constexpr auto componentsContours{2};

    // Setup expectations and behavior
    expectRemoveConnections();
    expectMorphOperations();
    onFindContours(componentsContours);
    EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(componentsContours).WillRepeatedly(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(componentsContours).WillRepeatedly(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(componentsContours).WillRepeatedly(Return(rect));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangleArea).Times(componentsContours).WillRepeatedly(Return(rectArea));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(0);

    // Detect components
    ImageMat image{};
    ASSERT_FALSE(mComponentDetection->detectComponents(image, image, mDummyConnections, false));

    // Number of components detected
    constexpr auto expectedComponents{0};
    const auto componentsDetected{mComponentDetection->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components are detected when there are no intersection points with connections.
 */
TEST_F(ComponentDetectionTest, detectsNoComponentsWhenNoConnectionPoints)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    const Rectangle rect{0, 0, 10, 10};
    // Area is equal to the minimum to consider rectangle as a component
    const auto rectArea{schematicSegmentation::ComponentDetection::cBoxMinArea};

    constexpr auto componentsContours{2};

    // Setup expectations and behavior
    expectRemoveConnections();
    expectMorphOperations();
    onFindContours(componentsContours);
    EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(componentsContours).WillRepeatedly(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(componentsContours).WillRepeatedly(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(componentsContours).WillRepeatedly(Return(rect));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangleArea).Times(componentsContours).WillRepeatedly(Return(rectArea));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(componentsContours).WillRepeatedly(Return(false));

    // Detect components
    ImageMat image{};
    ASSERT_FALSE(mComponentDetection->detectComponents(image, image, mDummyConnections, false));

    // Number of components detected
    constexpr auto expectedComponents{0};
    const auto componentsDetected{mComponentDetection->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that the images of components are saved during processing, when components are detected.
 */
TEST_F(ComponentDetectionTest, savesImagesWhenDetectsComponents)
{
    constexpr auto expectedComponents{1};
    constexpr auto saveImages{true};
    const ImageMat image{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(2 + expectedComponents);
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(2).WillRepeatedly(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(expectedComponents);
    setupDetectComponents(expectedComponents);

    // Detect components
    ImageMat img{};
    ASSERT_TRUE(mComponentDetection->detectComponents(img, img, mDummyConnections, saveImages));
}

/**
 * @brief Tests that the images of components are not saved during processing, when components are not detected.
 */
TEST_F(ComponentDetectionTest, savesNoImagesWhenNoDetectedComponents)
{
    constexpr auto expectedComponents{0};
    constexpr auto saveImages{true};
    const ImageMat image{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(2);
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(0);
    setupDetectComponents(expectedComponents);

    // Detect components
    ImageMat img{};
    ASSERT_FALSE(mComponentDetection->detectComponents(img, img, mDummyConnections, saveImages));
}

/**
 * @brief Tests that the connections are removed from image.
 */
TEST_F(ComponentDetectionTest, removesConnections)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(1);

    // Remove connections
    ImageMat img{};
    mComponentDetection->removeConnectionsFromImage(img, mDummyConnections);
}

/**
 * @brief Tests that the connections are not removed from image when there are no connections.
 */
TEST_F(ComponentDetectionTest, removesNoConnections)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(0);

    // Remove connections
    ImageMat img{};
    mComponentDetection->removeConnectionsFromImage(img, {});
}

/**
 * @brief Tests that, when checking a contour, a box is successfully returned.
 */
TEST_F(ComponentDetectionTest, returnsBoxWhenCheckContour)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    const Rectangle rect{0, 0, 10, 10};
    // Area is larger than the minimum
    const auto rectArea{schematicSegmentation::ComponentDetection::cBoxMinArea};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(1).WillOnce(Return(rect));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangleArea).Times(1).WillOnce(Return(rectArea));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(1).WillOnce(Return(true));

    // Check contour
    ImageMat img{};
    Contour contour{};
    // A box should be returned
    ASSERT_TRUE(mComponentDetection->checkContour(img, contour, mDummyConnections).has_value());
}

/**
 * @brief Tests that, when checking a contour, a box is successfully returned when there is at least one intersection
 * point with a connection.
 */
TEST_F(ComponentDetectionTest, returnsBoxWhenCheckContourAtLeastOneConnection)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    const Rectangle rect{0, 0, 10, 10};
    // Area is larger than the minimum
    const auto rectArea{schematicSegmentation::ComponentDetection::cBoxMinArea};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(1).WillOnce(Return(rect));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangleArea).Times(1).WillOnce(Return(rectArea));
    // Intersection point is detected with the second connection
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(2).WillOnce(Return(false)).WillOnce(Return(true));

    // Check contour
    ImageMat img{};
    Contour contour{};
    // Add more one connection to the vector
    setupDummyConnection();
    // A box should be returned
    ASSERT_TRUE(mComponentDetection->checkContour(img, contour, mDummyConnections).has_value());
}

/**
 * @brief Tests that, when checking a contour, a box is not returned when area is small.
 */
TEST_F(ComponentDetectionTest, returnsNoBoxWhenCheckContourSmallArea)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    const Rectangle rect{0, 0, 10, 10};
    // Area is smaller than the minimum
    const auto rectArea{schematicSegmentation::ComponentDetection::cBoxMinArea - 1};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(1).WillOnce(Return(rect));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangleArea).Times(1).WillOnce(Return(rectArea));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(0);

    // Check contour
    ImageMat img{};
    Contour contour{};
    // A box should not be returned
    ASSERT_FALSE(mComponentDetection->checkContour(img, contour, mDummyConnections).has_value());
}

/**
 * @brief Tests that, when checking a contour, a box is not returned when there is no connections.
 */
TEST_F(ComponentDetectionTest, returnsNoBoxWhenCheckContourNoConnections)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    const Rectangle rect{0, 0, 10, 10};
    // Area is larger than the minimum
    const auto rectArea{schematicSegmentation::ComponentDetection::cBoxMinArea};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(1).WillOnce(Return(rect));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangleArea).Times(1).WillOnce(Return(rectArea));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(0);

    // Check contour
    ImageMat img{};
    Contour contour{};
    // A box should not be returned
    ASSERT_FALSE(mComponentDetection->checkContour(img, contour, {}).has_value());
}

/**
 * @brief Tests that, when checking a contour, a box is not returned when there is no intersections points with
 * connections.
 */
TEST_F(ComponentDetectionTest, returnsNoBoxWhenCheckContourNoPoints)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    const Rectangle rect{0, 0, 10, 10};
    // Area is larger than the minimum
    const auto rectArea{schematicSegmentation::ComponentDetection::cBoxMinArea};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(1).WillOnce(Return(rect));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangleArea).Times(1).WillOnce(Return(rectArea));
    EXPECT_CALL(*mMockOpenCvWrapper, contains).Times(1).WillOnce(Return(false));

    // Check contour
    ImageMat img{};
    Contour contour{};
    // A box should not be returned
    ASSERT_FALSE(mComponentDetection->checkContour(img, contour, mDummyConnections).has_value());
}
