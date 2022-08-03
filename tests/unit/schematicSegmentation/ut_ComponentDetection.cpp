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
                                ? schematicSegmentation::ComponentDetection::cBoundingBoxMinArea
                                : (schematicSegmentation::ComponentDetection::cBoundingBoxMinArea - 1)};
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

protected:
    /** Component detection. */
    std::unique_ptr<schematicSegmentation::ComponentDetection> mComponentDetection;
    /** OpenCV wrapper. */
    std::shared_ptr<NiceMock<MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
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
    ASSERT_TRUE(mComponentDetection->detectComponents(image, image, false));

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
    ASSERT_TRUE(mComponentDetection->detectComponents(image, image, false));

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
    ASSERT_FALSE(mComponentDetection->detectComponents(image, image, false));

    // Number of components detected
    const auto componentsDetected{mComponentDetection->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components are detected when the area of the component blob is small.
 */
TEST_F(ComponentDetectionTest, detectsNoComponentsWhenSmallArea)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Area is smaller than the minimum to consider blob as a component
    constexpr auto contArea{schematicSegmentation::ComponentDetection::cBoundingBoxMinArea - 1};
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
    ASSERT_FALSE(mComponentDetection->detectComponents(image, image, false));

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
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(expectedComponents);
    setupDetectComponents(expectedComponents);

    // Detect components
    ImageMat img{};
    ASSERT_TRUE(mComponentDetection->detectComponents(img, img, saveImages));
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
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(0);
    setupDetectComponents(expectedComponents);

    // Detect components
    ImageMat img{};
    ASSERT_FALSE(mComponentDetection->detectComponents(img, img, saveImages));
}

/**
 * @brief Tests that component bounding box is built when the area of the component blob is large.
 */
TEST_F(ComponentDetectionTest, boundingBoxSingleComponent)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Area is equal to the minimum to consider blob as a component
    constexpr auto contArea{schematicSegmentation::ComponentDetection::cBoundingBoxMinArea};
    const Contours componentsContours{Contour{}};
    const Rectangle rect{0, 0, 100, 100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(1).WillOnce(Return(contArea));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(1).WillOnce(Return(rect));

    // Bounding boxes
    ImageMat img{};
    mComponentDetection->boundingBoxComponents(componentsContours, img);

    // Number of components detected
    constexpr auto expectedComponents{1};
    const auto componentsDetected{mComponentDetection->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that components bounding boxes are built when the area of the components blobs are large.
 */
TEST_F(ComponentDetectionTest, boundingBoxMultipleComponents)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Area is equal to the minimum to consider blob as a component
    constexpr auto contArea{schematicSegmentation::ComponentDetection::cBoundingBoxMinArea};
    const Contours componentsContours{Contour{}, Contour{}};
    const Rectangle rect{0, 0, 100, 100};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(2).WillRepeatedly(Return(contArea));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(2).WillRepeatedly(Return(rect));

    // Bounding boxes
    ImageMat img{};
    mComponentDetection->boundingBoxComponents(componentsContours, img);

    // Number of components detected
    constexpr auto expectedComponents{2};
    const auto componentsDetected{mComponentDetection->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components bounding boxes are built when there are no components blobs.
 */
TEST_F(ComponentDetectionTest, noBoundingBoxComponentWhenNoBlobs)
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
    mComponentDetection->boundingBoxComponents(componentsBlobs, img);

    // Number of components detected
    constexpr auto expectedComponents{0};
    const auto componentsDetected{mComponentDetection->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}

/**
 * @brief Tests that no components bounding boxes are built when the area of the component blob is small.
 */
TEST_F(ComponentDetectionTest, noBoundingBoxComponentWhenSmallArea)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    // Area is smaller than the minimum to consider blob as a component
    constexpr auto contArea{schematicSegmentation::ComponentDetection::cBoundingBoxMinArea - 1};
    const Contours componentsBlobs{Contour{}};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, contourArea).Times(1).WillOnce(Return(contArea));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(0);

    // Bounding boxes
    ImageMat img{};
    mComponentDetection->boundingBoxComponents(componentsBlobs, img);

    // Number of components detected
    constexpr auto expectedComponents{0};
    const auto componentsDetected{mComponentDetection->getDetectedComponents().size()};
    EXPECT_EQ(componentsDetected, expectedComponents);
}
