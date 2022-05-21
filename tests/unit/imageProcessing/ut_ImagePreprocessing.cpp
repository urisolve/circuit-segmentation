/**
 * @file
 */

#include "imageProcessing/ImagePreprocessing.h"
#include "imageProcessing/MockImageProcUtils.h"
#include "logging/Logger.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <opencv2/core.hpp>

using namespace testing;

/**
 * @brief Test class of ImagePreprocessing.
 */
class ImagePreprocessingTest : public ::testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mMockImageProcUtils = std::make_shared<NaggyMock<circuitSegmentation::imageProcessing::MockImageProcUtils>>();
        mLogger = std::make_shared<circuitSegmentation::logging::Logger>(std::cout);

        mImagePreprocessing = std::make_unique<circuitSegmentation::imageProcessing::ImagePreprocessing>(
            mMockImageProcUtils, mLogger, false);
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /** Image preprocessing. */
    std::unique_ptr<circuitSegmentation::imageProcessing::ImagePreprocessing> mImagePreprocessing;
    /** Image processing utils. */
    std::shared_ptr<NaggyMock<circuitSegmentation::imageProcessing::MockImageProcUtils>> mMockImageProcUtils;
    /** Logger. */
    std::shared_ptr<circuitSegmentation::logging::Logger> mLogger;

    /** Image to be used in tests. */
    cv::Mat mTestImage{300, 300, CV_8UC3, cv::Scalar(128, 128, 128)};
    /** Image file to be used in tests. */
    const std::string cTestImageFile{"test_image.png"};
    /** Window name to be used in tests. */
    const std::string cTestImageWindow{"Test image"};
};

/**
 * @brief Tests that the images obtained during the processing are saved when the flag to save images is true.
 */
TEST_F(ImagePreprocessingTest, preprocessImageSave)
{
    // Set flag to save images
    mImagePreprocessing->setSaveImages(true);

    // Setup behavior
    ON_CALL(*mMockImageProcUtils, writeImage(_, _)).WillByDefault(Return(true));

    // Setup expectations
    EXPECT_CALL(*mMockImageProcUtils, writeImage(_, _)).Times(4);

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
    EXPECT_CALL(*mMockImageProcUtils, writeImage(_, _)).Times(0);

    // Preprocess image
    mImagePreprocessing->preprocessImage(mTestImage);
}

/**
 * @brief Tests that the image is resized when it is larger than the maximum dimension.
 */
TEST_F(ImagePreprocessingTest, resizesImageSquare)
{
    // Large image
    cv::Mat image{circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim + 100,
                  circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim + 100,
                  CV_8UC3,
                  cv::Scalar(128, 128, 128)};

    // Resize image
    mImagePreprocessing->resizeImage(image);

    // Expect the image is resized
    EXPECT_GE(circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim, image.size().width);
    EXPECT_GE(circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim, image.size().height);
}

/**
 * @brief Tests that the image is resized when its width is larger than the maximum dimension.
 */
TEST_F(ImagePreprocessingTest, resizesImageWidth)
{
    // Large image
    cv::Mat image{circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim - 100,
                  circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim + 100,
                  CV_8UC3,
                  cv::Scalar(128, 128, 128)};

    // Resize image
    mImagePreprocessing->resizeImage(image);

    // Expect the image is resized
    EXPECT_GE(circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim, image.size().width);
    EXPECT_GE(circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim, image.size().height);
}

/**
 * @brief Tests that the image is resized when its height is larger than the maximum dimension.
 */
TEST_F(ImagePreprocessingTest, resizesImageHeight)
{
    // Large image
    cv::Mat image{circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim + 100,
                  circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim - 100,
                  CV_8UC3,
                  cv::Scalar(128, 128, 128)};

    // Resize image
    mImagePreprocessing->resizeImage(image);

    // Expect the image is resized
    EXPECT_GE(circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim, image.size().width);
    EXPECT_GE(circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim, image.size().height);
}

/**
 * @brief Tests that the image is not resized when it is smaller than or equal to the maximum dimension.
 */
TEST_F(ImagePreprocessingTest, doesNotResizeImage)
{
    // Small image
    cv::Mat image{circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim - 100,
                  circuitSegmentation::imageProcessing::ImagePreprocessing::cResizeDim - 100,
                  CV_8UC3,
                  cv::Scalar(128, 128, 128)};

    // Expected values
    const auto expectWidth = image.size().width;
    const auto expectHeight = image.size().height;

    // Resize image
    mImagePreprocessing->resizeImage(image);

    // Expect the image is not resized
    EXPECT_EQ(expectWidth, image.size().width);
    EXPECT_EQ(expectHeight, image.size().height);
}

/**
 * @brief Tests that the convertImageToGray() method does not throw.
 */
TEST_F(ImagePreprocessingTest, convertImageToGrayDoesNotThrow)
{
    EXPECT_NO_THROW(mImagePreprocessing->convertImageToGray(mTestImage));
}

/**
 * @brief Tests that the blurImage() method does not throw.
 */
TEST_F(ImagePreprocessingTest, blurImageDoesNotThrow)
{
    EXPECT_NO_THROW(mImagePreprocessing->blurImage(mTestImage));
}

/**
 * @brief Tests that the edgesImage() method does not throw.
 */
TEST_F(ImagePreprocessingTest, edgesImageDoesNotThrow)
{
    // Image should be filtered before the edge detection
    mImagePreprocessing->convertImageToGray(mTestImage);
    mImagePreprocessing->blurImage(mTestImage);

    EXPECT_NO_THROW(mImagePreprocessing->edgesImage(mTestImage));
}

/**
 * @brief Tests the setter of the flag to save images.
 */
TEST_F(ImagePreprocessingTest, setsSaveImages)
{
    const auto saveImages{true};
    mImagePreprocessing->setSaveImages(saveImages);

    EXPECT_EQ(saveImages, mImagePreprocessing->getSaveImages());
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
