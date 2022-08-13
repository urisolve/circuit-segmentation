/**
 * @file
 */

#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include "schematicSegmentation/LabelDetection.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace circuitSegmentation;
using namespace circuitSegmentation::computerVision;

/**
 * @brief Test class of LabelDetection.
 */
class LabelDetectionTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockOpenCvWrapper = std::make_shared<NiceMock<MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);

        mLabelDetection = std::make_unique<schematicSegmentation::LabelDetection>(mMockOpenCvWrapper, mLogger);

        setupDummyComponent();
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
     * @brief Expects call to remove elements from image.
     */
    void expectRemoveElements()
    {
        // Setup expectations
        // EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(1);
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
            .Times(2)
            .WillRepeatedly(Return(kernel));
        EXPECT_CALL(*mMockOpenCvWrapper, morphologyEx(_, _, OpenCvWrapper::MorphTypes::MORPH_CLOSE, _, _)).Times(1);
        EXPECT_CALL(*mMockOpenCvWrapper, morphologyEx(_, _, OpenCvWrapper::MorphTypes::MORPH_OPEN, _, _)).Times(1);
    }

    /**
     * @brief Sets the behavior when checking contour.
     *
     * @param numLabelsDetected Number of labels detected.
     */
    void onCheckContour(const unsigned int numLabelsDetected)
    {
        constexpr auto imgWidth{100};
        constexpr auto imgHeight{100};
        const Rectangle rect{0, 0, 10, 10};
        // Area is larger than the minimum to consider rectangle as a label, if there are labels to detect
        const auto rectArea{numLabelsDetected > 0 ? schematicSegmentation::LabelDetection::cBoxMinArea
                                                  : (schematicSegmentation::LabelDetection::cBoxMinArea - 1)};

        // Setup expectations and behavior
        ON_CALL(*mMockOpenCvWrapper, getImageWidth).WillByDefault(Return(imgWidth));
        ON_CALL(*mMockOpenCvWrapper, getImageHeight).WillByDefault(Return(imgHeight));
        ON_CALL(*mMockOpenCvWrapper, boundingRect).WillByDefault(Return(rect));
        ON_CALL(*mMockOpenCvWrapper, rectangleArea).WillByDefault(Return(rectArea));
        EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(numLabelsDetected);
        EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(numLabelsDetected);
        EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(numLabelsDetected);
        EXPECT_CALL(*mMockOpenCvWrapper, rectangleArea).Times(numLabelsDetected);
    }

    /**
     * @brief Sets up detection of labels.
     *
     * @param numLabelsDetected Number of labels detected.
     */
    void setupDetectLabels(const unsigned int numLabelsDetected)
    {
        // Setup expectations and behavior
        expectRemoveElements();
        expectMorphOperations();
        onFindContours(numLabelsDetected);
        onCheckContour(numLabelsDetected);

        EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
    }

    /**
     * @brief Sets up a dummy component.
     */
    void setupDummyComponent()
    {
        // Bounding box
        constexpr auto x{5};
        constexpr auto y{5};
        const Rectangle rectangle{x, y, cDimension, cDimension};

        // Dummy component
        circuit::Component dummyComponent{};
        dummyComponent.mBoundingBox = rectangle;

        // Set
        mDummyComponents.push_back(dummyComponent);
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
    /** Dimension for width and height for boxes of components to be used in tests. */
    static constexpr auto cDimension{10};

    /** Label detection. */
    std::unique_ptr<schematicSegmentation::LabelDetection> mLabelDetection;
    /** OpenCV wrapper. */
    std::shared_ptr<NiceMock<MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
    /** Dummy components to be used in tests. */
    std::vector<circuit::Component> mDummyComponents;
    /** Dummy connections to be used in tests. */
    std::vector<circuit::Connection> mDummyConnections;
};

/**
 * @brief Tests that a single label is detected.
 */
TEST_F(LabelDetectionTest, detectsSingleLabel)
{
    constexpr auto expectedLabels{1};

    // Setup expectations and behavior
    setupDetectLabels(expectedLabels);

    // Detect labels
    ImageMat image{};
    ASSERT_TRUE(mLabelDetection->detectLabels(image, image, mDummyComponents, mDummyConnections, false));

    // Number of labels detected
    const auto labelsDetected{mLabelDetection->getDetectedLabels().size()};
    EXPECT_EQ(labelsDetected, expectedLabels);
}

/**
 * @brief Tests that multiple labels are detected.
 */
TEST_F(LabelDetectionTest, detectsMultipleLabels)
{
    constexpr auto expectedLabels{2};

    // Setup expectations and behavior
    setupDetectLabels(expectedLabels);

    // Detect labels
    ImageMat image{};
    ASSERT_TRUE(mLabelDetection->detectLabels(image, image, mDummyComponents, mDummyConnections, false));

    // Number of labels detected
    const auto labelsDetected{mLabelDetection->getDetectedLabels().size()};
    EXPECT_EQ(labelsDetected, expectedLabels);
}

/**
 * @brief Tests that no labels are detected when there are no labels contours found.
 */
TEST_F(LabelDetectionTest, detectsNoLabelsWhenNoContours)
{
    constexpr auto expectedLabels{0};

    // Setup expectations and behavior
    setupDetectLabels(expectedLabels);

    // Detect labels
    ImageMat image{};
    ASSERT_FALSE(mLabelDetection->detectLabels(image, image, mDummyComponents, mDummyConnections, false));

    // Number of labels detected
    const auto labelsDetected{mLabelDetection->getDetectedLabels().size()};
    EXPECT_EQ(labelsDetected, expectedLabels);
}

/**
 * @brief Tests that no labels are detected when the area of the label is small.
 */
TEST_F(LabelDetectionTest, detectsNoLabelsWhenSmallArea)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    const Rectangle rect{0, 0, 10, 10};
    // Area is smaller than the minimum to consider rectangle as a label
    const auto rectArea{schematicSegmentation::LabelDetection::cBoxMinArea - 1};

    constexpr auto labelsContours{2};

    // Setup expectations and behavior
    expectRemoveElements();
    expectMorphOperations();
    onFindContours(labelsContours);
    EXPECT_CALL(*mMockOpenCvWrapper, findContours).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(labelsContours).WillRepeatedly(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(labelsContours).WillRepeatedly(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(labelsContours).WillRepeatedly(Return(rect));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangleArea).Times(labelsContours).WillRepeatedly(Return(rectArea));

    // Detect labels
    ImageMat image{};
    ASSERT_FALSE(mLabelDetection->detectLabels(image, image, mDummyComponents, mDummyConnections, false));

    // Number of labels detected
    constexpr auto expectedLabels{0};
    const auto labelsDetected{mLabelDetection->getDetectedLabels().size()};
    EXPECT_EQ(labelsDetected, expectedLabels);
}

/**
 * @brief Tests that the images of labels are saved during processing, when labels are detected.
 */
TEST_F(LabelDetectionTest, savesImagesWhenDetectsLabels)
{
    constexpr auto expectedLabels{1};
    constexpr auto saveImages{true};
    const ImageMat image{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(3 + expectedLabels);
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(2).WillRepeatedly(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(1 + expectedLabels);
    setupDetectLabels(expectedLabels);

    // Detect labels
    ImageMat img{};
    ASSERT_TRUE(mLabelDetection->detectLabels(img, img, mDummyComponents, mDummyConnections, saveImages));
}

/**
 * @brief Tests that the images of labels are not saved during processing, when labels are not detected.
 */
TEST_F(LabelDetectionTest, savesNoImagesWhenNoDetectedLabels)
{
    constexpr auto expectedLabels{0};
    constexpr auto saveImages{true};
    const ImageMat image{};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(3);
    EXPECT_CALL(*mMockOpenCvWrapper, cloneImage).Times(1).WillOnce(Return(image));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(1);
    setupDetectLabels(expectedLabels);

    // Detect components
    ImageMat img{};
    ASSERT_FALSE(mLabelDetection->detectLabels(img, img, mDummyComponents, mDummyConnections, saveImages));
}

/**
 * @brief Tests that the elements are removed from image.
 */
TEST_F(LabelDetectionTest, removesElements)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(1);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(1);

    // Remove elements
    ImageMat img{};
    mLabelDetection->removeElementsFromImage(img, mDummyComponents, mDummyConnections);
}

/**
 * @brief Tests that the elements are not removed from image when there are no elements.
 */
TEST_F(LabelDetectionTest, removesNoElements)
{
    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, rectangle).Times(0);
    EXPECT_CALL(*mMockOpenCvWrapper, drawContours).Times(0);

    // Remove elements
    ImageMat img{};
    mLabelDetection->removeElementsFromImage(img, {}, {});
}

/**
 * @brief Tests that, when checking a contour, a box is successfully returned.
 */
TEST_F(LabelDetectionTest, returnsBoxWhenCheckContour)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    const Rectangle rect{0, 0, 10, 10};
    // Area is larger than the minimum
    const auto rectArea{schematicSegmentation::LabelDetection::cBoxMinArea};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(1).WillOnce(Return(rect));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangleArea).Times(1).WillOnce(Return(rectArea));

    // Check contour
    ImageMat img{};
    Contour contour{};
    // A box should be returned
    ASSERT_TRUE(mLabelDetection->checkContour(img, contour).has_value());
}

/**
 * @brief Tests that, when checking a contour, a box is not returned when area is small.
 */
TEST_F(LabelDetectionTest, returnsNoBoxWhenCheckContourSmallArea)
{
    constexpr auto imgWidth{100};
    constexpr auto imgHeight{100};
    const Rectangle rect{0, 0, 10, 10};
    // Area is smaller than the minimum
    const auto rectArea{schematicSegmentation::LabelDetection::cBoxMinArea - 1};

    // Setup expectations and behavior
    EXPECT_CALL(*mMockOpenCvWrapper, getImageWidth).Times(1).WillOnce(Return(imgWidth));
    EXPECT_CALL(*mMockOpenCvWrapper, getImageHeight).Times(1).WillOnce(Return(imgHeight));
    EXPECT_CALL(*mMockOpenCvWrapper, boundingRect).Times(1).WillOnce(Return(rect));
    EXPECT_CALL(*mMockOpenCvWrapper, rectangleArea).Times(1).WillOnce(Return(rectArea));

    // Check contour
    ImageMat img{};
    Contour contour{};
    // A box should not be returned
    ASSERT_FALSE(mLabelDetection->checkContour(img, contour).has_value());
}
