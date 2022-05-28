/**
 * @file
 */

#include "computerVision/OpenCvWrapper.h"
#include <gtest/gtest.h>

using namespace circuitSegmentation::computerVision;
/**
 * @brief Test class of OpenCvWrapper.
 */
class OpenCvWrapperTest : public ::testing::Test
{
protected:
    /**
     * @brief Test suite setup.
     */
    void SetUp() override
    {
        mOpenCvWrapper = std::make_unique<OpenCvWrapper>();
    }

    /**
     * @brief Test suite teardown.
     */
    void TearDown() override {}

    /** Existent image file path to be used in tests. */
    const std::string cExistentImageFilePath{std::string(TESTS_DATA_PATH) + "circuit-comp-1.png"};
    /** Nonexistent image file path to be used in tests. */
    const std::string cNonExistentImageFilePath{std::string(TESTS_DATA_PATH) + "nonexistent.png"};

    /** OpenCV wrapper. */
    std::unique_ptr<OpenCvWrapper> mOpenCvWrapper;
    /** Width of the image to be used in tests. */
    const int cTestImageWidth{300};
    /** Height of the image to be used in tests. */
    const int cTestImageHeight{300};
    /** Image to be used in tests. */
    ImageMat mTestImage{cTestImageHeight, cTestImageWidth, CV_8UC3, cv::Scalar(128, 128, 128)};
};

/**
 * @brief Tests the log mode setted.
 */
TEST_F(OpenCvWrapperTest, setsLogMode)
{
    // Silent mode
    auto silent{true};

    mOpenCvWrapper->setLogMode(silent);
    EXPECT_EQ(silent, mOpenCvWrapper->getLogMode());

    silent = false;

    mOpenCvWrapper->setLogMode(silent);
    EXPECT_EQ(silent, mOpenCvWrapper->getLogMode());
}

/**
 * @brief Tests if image is written successfully.
 */
TEST_F(OpenCvWrapperTest, writesImageSuccessfully)
{
    const auto fileName{"test_image.png"};

    // Write image
    const auto result = mOpenCvWrapper->writeImage(fileName, mTestImage);

    EXPECT_EQ(true, result);
}

/**
 * @brief Tests if image is written unsuccessfully when it is used an incorrect file name.
 */
TEST_F(OpenCvWrapperTest, writesImageUnsuccessfully)
{
    const auto fileName{""};

    // Write image
    const auto result = mOpenCvWrapper->writeImage(fileName, mTestImage);

    EXPECT_EQ(false, result);
}

/**
 * @brief Tests if image is read successfully.
 */
TEST_F(OpenCvWrapperTest, readsImageSuccessfully)
{
    const auto filePath{cExistentImageFilePath};

    // Read image
    auto image = mOpenCvWrapper->readImage(filePath);

    // Image cannot be empty
    EXPECT_EQ(false, mOpenCvWrapper->isImageEmpty(image));
}

/**
 * @brief Tests if image is read unsuccessfully when it is used an incorrect file name.
 */
TEST_F(OpenCvWrapperTest, readsImageUnsuccessfully)
{
    const auto filePath{cNonExistentImageFilePath};

    // Read image
    auto image = mOpenCvWrapper->readImage(filePath);

    // Image is empty
    EXPECT_EQ(true, mOpenCvWrapper->isImageEmpty(image));
}

/**
 * @brief Tests that the method to clone image does not throw an exception.
 */
TEST_F(OpenCvWrapperTest, cloneImageNoThrow)
{
    EXPECT_NO_THROW(mOpenCvWrapper->cloneImage(mTestImage));
}

/**
 * @brief Tests if an image is empty when it is empty.
 */
TEST_F(OpenCvWrapperTest, checksImageEmpty)
{
    ImageMat image{};

    // Image is empty
    EXPECT_EQ(true, mOpenCvWrapper->isImageEmpty(image));
}

/**
 * @brief Tests if an image is empty when it is not empty.
 */
TEST_F(OpenCvWrapperTest, checksImageNotEmpty)
{
    ImageMat image{mTestImage};

    // Image is not empty
    EXPECT_EQ(false, mOpenCvWrapper->isImageEmpty(image));
}

/**
 * @brief Tests the resizing of an image.
 */
TEST_F(OpenCvWrapperTest, resizesImage)
{
    // Image resized
    ImageMat imageResized{};
    // Scale factor
    const auto scale{0.5};

    // Resize image
    mOpenCvWrapper->resizeImage(mTestImage, imageResized, scale);

    // Dimensions of original image
    const auto width = mOpenCvWrapper->getImageWidth(mTestImage);
    const auto height = mOpenCvWrapper->getImageHeight(mTestImage);

    // Dimensions of image resized
    const auto resizedWidth = mOpenCvWrapper->getImageWidth(imageResized);
    const auto resizedHeight = mOpenCvWrapper->getImageHeight(imageResized);

    // Expected dimensions of image resized
    const auto expectWidth = static_cast<int>(width * scale);
    const auto expectHeight = static_cast<int>(height * scale);

    // Expect the image is resized correctly
    EXPECT_EQ(resizedWidth, expectWidth);
    EXPECT_EQ(resizedHeight, expectHeight);
}

/**
 * @brief Tests if the image width is correct.
 */
TEST_F(OpenCvWrapperTest, checksImageWidth)
{
    EXPECT_EQ(cTestImageWidth, mOpenCvWrapper->getImageWidth(mTestImage));
}

/**
 * @brief Tests if the image height is correct.
 */
TEST_F(OpenCvWrapperTest, checksImageHeight)
{
    EXPECT_EQ(cTestImageHeight, mOpenCvWrapper->getImageHeight(mTestImage));
}

/**
 * @brief Tests that the method for conversion to grayscale does not throw an exception.
 */
TEST_F(OpenCvWrapperTest, convertImageToGrayNoThrow)
{
    ImageMat image{};

    EXPECT_NO_THROW(mOpenCvWrapper->convertImageToGray(mTestImage, image));
}

/**
 * @brief Tests that the method for Gaussian blur image does not throw an exception.
 */
TEST_F(OpenCvWrapperTest, gaussianBlurImageNoThrow)
{
    ImageMat image{};

    EXPECT_NO_THROW(mOpenCvWrapper->gaussianBlurImage(mTestImage, image, 5));
}

/**
 * @brief Tests that the method for adaptive thresholding the image does not throw an exception.
 */
TEST_F(OpenCvWrapperTest, adaptiveThresholdImageNoThrow)
{
    // Image to be processed
    ImageMat image{};

    // Image should be filtered before the adaptive thresholding
    mOpenCvWrapper->convertImageToGray(mTestImage, image);
    mOpenCvWrapper->gaussianBlurImage(image, image, 5);

    // Adaptive threshold parameters
    const double maxValue{255};
    const OpenCvWrapper::AdaptiveThresholdAlgorithm adaptiveMethod{
        OpenCvWrapper::AdaptiveThresholdAlgorithm::ADAPTIVE_THRESH_GAUSSIAN};
    const OpenCvWrapper::ThresholdOperations thresholdOp{OpenCvWrapper::ThresholdOperations::THRESH_BINARY};
    const int blockSize{21};
    const double subConst{10};

    EXPECT_NO_THROW(mOpenCvWrapper->adaptiveThresholdImage(
        image, image, maxValue, adaptiveMethod, thresholdOp, blockSize, subConst));
}

/**
 * @brief Tests that the method for Canny edge detector does not throw an exception.
 */
TEST_F(OpenCvWrapperTest, cannyEdgeImageNoThrow)
{
    // Image to be processed
    ImageMat image{};

    // Image should be filtered before the edge detection
    mOpenCvWrapper->convertImageToGray(mTestImage, image);
    mOpenCvWrapper->gaussianBlurImage(image, image, 5);

    // Canny edge parameters
    const double thresh1{50};
    const double thresh2{200};
    const int apertureSize{3};

    EXPECT_NO_THROW(mOpenCvWrapper->cannyEdgeImage(image, image, thresh1, thresh2, apertureSize));
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
