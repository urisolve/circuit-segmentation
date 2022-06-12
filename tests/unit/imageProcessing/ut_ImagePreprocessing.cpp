/**
 * @file
 */

#include "imageProcessing/ImagePreprocessing.h"
#include "logging/Logger.h"
#include "mocks/computerVision/MockOpenCvWrapper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace circuitSegmentation;

/**
 * @brief Test class of ImagePreprocessing.
 */
class ImagePreprocessingTest : public testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockOpenCvWrapper = std::make_shared<NiceMock<computerVision::MockOpenCvWrapper>>();
        mLogger = std::make_shared<logging::Logger>(std::cout);

        mImagePreprocessing = std::make_unique<imageProcessing::ImagePreprocessing>(mMockOpenCvWrapper, mLogger, false);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

protected:
    /** Image preprocessing. */
    std::unique_ptr<imageProcessing::ImagePreprocessing> mImagePreprocessing;
    /** OpenCV wrapper. */
    std::shared_ptr<NiceMock<computerVision::MockOpenCvWrapper>> mMockOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<circuitSegmentation::logging::Logger> mLogger;

    /** Image to be used in tests. */
    computerVision::ImageMat mTestImage{};
};

/**
 * @brief Tests that the images obtained during the processing are saved when the flag to save images is true.
 */
TEST_F(ImagePreprocessingTest, preprocessImageSave)
{
    // Set flag to save images
    mImagePreprocessing->setSaveImages(true);

    // Setup behavior
    ON_CALL(*mMockOpenCvWrapper, writeImage).WillByDefault(Return(true));

    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(4);

    // Preprocess image
    mImagePreprocessing->preprocessImage(mTestImage);
}

/**
 * @brief Tests that the images obtained during the processing are not saved when the flag to save images is false.
 */
TEST_F(ImagePreprocessingTest, preprocessImageNotSave)
{
    // Set flag to save images
    mImagePreprocessing->setSaveImages(false);

    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, writeImage).Times(0);

    // Preprocess image
    mImagePreprocessing->preprocessImage(mTestImage);
}

/**
 * @brief Tests that the flag to save images is defined correctly.
 */
TEST_F(ImagePreprocessingTest, setsSaveImages)
{
    const auto saveImages{true};
    mImagePreprocessing->setSaveImages(saveImages);

    EXPECT_EQ(saveImages, mImagePreprocessing->getSaveImages());
}

/**
 * @brief Tests that the image is resized when it is larger than the maximum dimension.
 */
TEST_F(ImagePreprocessingTest, resizesImageSquare)
{
    // Setup behavior
    ON_CALL(*mMockOpenCvWrapper, getImageWidth)
        .WillByDefault(Return(imageProcessing::ImagePreprocessing::cResizeDim + 100));
    ON_CALL(*mMockOpenCvWrapper, getImageHeight)
        .WillByDefault(Return(imageProcessing::ImagePreprocessing::cResizeDim + 100));

    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, resizeImage).Times(1);

    // Resize image
    mImagePreprocessing->resizeImage(mTestImage);
}

/**
 * @brief Tests that the image is resized when its width is larger than the maximum dimension.
 */
TEST_F(ImagePreprocessingTest, resizesImageWidth)
{
    // Setup behavior
    ON_CALL(*mMockOpenCvWrapper, getImageWidth)
        .WillByDefault(Return(imageProcessing::ImagePreprocessing::cResizeDim + 100));
    ON_CALL(*mMockOpenCvWrapper, getImageHeight)
        .WillByDefault(Return(imageProcessing::ImagePreprocessing::cResizeDim - 100));

    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, resizeImage).Times(1);

    // Resize image
    mImagePreprocessing->resizeImage(mTestImage);
}

/**
 * @brief Tests that the image is resized when its height is larger than the maximum dimension.
 */
TEST_F(ImagePreprocessingTest, resizesImageHeight)
{
    // Setup behavior
    ON_CALL(*mMockOpenCvWrapper, getImageWidth)
        .WillByDefault(Return(imageProcessing::ImagePreprocessing::cResizeDim - 100));
    ON_CALL(*mMockOpenCvWrapper, getImageHeight)
        .WillByDefault(Return(imageProcessing::ImagePreprocessing::cResizeDim + 100));

    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, resizeImage).Times(1);

    // Resize image
    mImagePreprocessing->resizeImage(mTestImage);
}

/**
 * @brief Tests that the image is not resized when it is smaller than or equal to the maximum dimension.
 */
TEST_F(ImagePreprocessingTest, doesNotResizeImage)
{
    // Setup behavior
    ON_CALL(*mMockOpenCvWrapper, getImageWidth)
        .WillByDefault(Return(imageProcessing::ImagePreprocessing::cResizeDim - 100));
    ON_CALL(*mMockOpenCvWrapper, getImageHeight)
        .WillByDefault(Return(imageProcessing::ImagePreprocessing::cResizeDim - 100));

    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, resizeImage).Times(0);

    // Resize image
    mImagePreprocessing->resizeImage(mTestImage);
}

/**
 * @brief Tests that the image is converted to grayscale.
 */
TEST_F(ImagePreprocessingTest, convertsImageToGray)
{
    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, convertImageToGray).Times(1);

    // Convert image to grayscale
    mImagePreprocessing->convertImageToGray(mTestImage);
}

/**
 * @brief Tests that the image is blurred.
 */
TEST_F(ImagePreprocessingTest, blursImage)
{
    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, gaussianBlurImage).Times(1);

    // Blur image
    mImagePreprocessing->blurImage(mTestImage);
}

/**
 * @brief Tests that the threshold operation is applied to the image.
 */
TEST_F(ImagePreprocessingTest, thresholdingImage)
{
    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, adaptiveThresholdImage).Times(1);

    // Thresholding image
    mImagePreprocessing->thresholdImage(mTestImage);
}

/**
 * @brief Tests that the morphological opening operation is applied to the image.
 */
TEST_F(ImagePreprocessingTest, morphologicalOpenImage)
{
    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, morphologyEx).Times(1);

    // Morphological opening image
    mImagePreprocessing->morphologicalOpenImage(mTestImage);
}

/**
 * @brief Tests that the edges of image are detected.
 */
TEST_F(ImagePreprocessingTest, detectsImageEdges)
{
    // Setup expectations
    EXPECT_CALL(*mMockOpenCvWrapper, cannyEdgeImage).Times(1);

    // Detect edges
    mImagePreprocessing->edgesImage(mTestImage);
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
