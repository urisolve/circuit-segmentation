/**
 * @file
 */

#include "computerVision/OpenCvWrapper.h"
#include <gtest/gtest.h>
#include <vector>

using namespace circuitSegmentation::computerVision;

/**
 * @brief Test class of OpenCvWrapper.
 */
class OpenCvWrapperTest : public testing::Test
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

protected:
    /** Existent image file path to be used in tests. */
    const std::string cExistentImageFilePath{std::string(TESTS_DATA_PATH) + "circuit-1.png"};
    /** Nonexistent image file path to be used in tests. */
    const std::string cNonExistentImageFilePath{std::string(TESTS_DATA_PATH) + "nonexistent.png"};

    /** OpenCV wrapper. */
    std::unique_ptr<OpenCvWrapper> mOpenCvWrapper;
    /** Width of the image to be used in tests. */
    const int cTestImageWidth{300};
    /** Height of the image to be used in tests. */
    const int cTestImageHeight{300};
    /** Image with 3 channels to be used in tests. */
    ImageMat mTestImage3chn{cTestImageHeight, cTestImageWidth, CV_8UC3, cv::Scalar(128, 128, 128)};
    /** Image with 1 channel to be used in tests. */
    ImageMat mTestImage1chn{cTestImageHeight, cTestImageWidth, CV_8UC1, cv::Scalar(128, 128, 128)};
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
    const auto result = mOpenCvWrapper->writeImage(fileName, mTestImage3chn);

    EXPECT_TRUE(result);
}

/**
 * @brief Tests if image is written unsuccessfully when it is used an incorrect file name.
 */
TEST_F(OpenCvWrapperTest, writesImageUnsuccessfully)
{
    const auto fileName{""};

    // Write image
    const auto result = mOpenCvWrapper->writeImage(fileName, mTestImage3chn);

    EXPECT_FALSE(result);
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
    EXPECT_FALSE(mOpenCvWrapper->isImageEmpty(image));
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
    EXPECT_TRUE(mOpenCvWrapper->isImageEmpty(image));
}

/**
 * @brief Tests that the method to clone image does not throw an exception.
 */
TEST_F(OpenCvWrapperTest, cloneImageNoThrow)
{
    EXPECT_NO_THROW(mOpenCvWrapper->cloneImage(mTestImage3chn));
}

/**
 * @brief Tests if an image is empty when it is empty.
 */
TEST_F(OpenCvWrapperTest, checksImageEmpty)
{
    ImageMat image{};

    // Image is empty
    EXPECT_TRUE(mOpenCvWrapper->isImageEmpty(image));
}

/**
 * @brief Tests if an image is empty when it is not empty.
 */
TEST_F(OpenCvWrapperTest, checksImageNotEmpty)
{
    // Image is not empty
    EXPECT_FALSE(mOpenCvWrapper->isImageEmpty(mTestImage3chn));
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
    mOpenCvWrapper->resizeImage(mTestImage3chn, imageResized, scale);

    // Dimensions of initial image
    const auto width = mOpenCvWrapper->getImageWidth(mTestImage3chn);
    const auto height = mOpenCvWrapper->getImageHeight(mTestImage3chn);

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
 * @brief Tests if the image dimensions (width and height) are correct.
 */
TEST_F(OpenCvWrapperTest, checksImageDimensions)
{
    EXPECT_EQ(cTestImageWidth, mOpenCvWrapper->getImageWidth(mTestImage3chn));
    EXPECT_EQ(cTestImageHeight, mOpenCvWrapper->getImageHeight(mTestImage3chn));
}

/**
 * @brief Tests that the method for conversion to grayscale does not throw an exception.
 */
TEST_F(OpenCvWrapperTest, convertImageToGrayNoThrow)
{
    EXPECT_NO_THROW(mOpenCvWrapper->convertImageToGray(mTestImage3chn, mTestImage3chn));
}

/**
 * @brief Tests that the method for Gaussian blur image does not throw an exception.
 */
TEST_F(OpenCvWrapperTest, gaussianBlurImageNoThrow)
{
    EXPECT_NO_THROW(mOpenCvWrapper->gaussianBlurImage(mTestImage3chn, mTestImage3chn, 5));
}

/**
 * @brief Tests that the method for adaptive thresholding the image does not throw an exception.
 */
TEST_F(OpenCvWrapperTest, adaptiveThresholdImageNoThrow)
{
    // Image to be processed
    ImageMat image{};

    // Image should be filtered before the adaptive thresholding
    mOpenCvWrapper->convertImageToGray(mTestImage3chn, image);
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
    mOpenCvWrapper->convertImageToGray(mTestImage3chn, image);
    mOpenCvWrapper->gaussianBlurImage(image, image, 5);

    // Canny edge parameters
    const double thresh1{50};
    const double thresh2{200};
    const int apertureSize{3};

    EXPECT_NO_THROW(mOpenCvWrapper->cannyEdgeImage(image, image, thresh1, thresh2, apertureSize));
}

/**
 * @brief Tests that the structuring element is not empty.
 */
TEST_F(OpenCvWrapperTest, getStructuringElementNotEmpty)
{
    // Structuring element parameters
    const OpenCvWrapper::MorphShapes shape{OpenCvWrapper::MorphShapes::MORPH_RECT};
    const unsigned int size{3};

    // Kernel
    ImageMat kernel{mOpenCvWrapper->getStructuringElement(shape, size)};

    EXPECT_FALSE(mOpenCvWrapper->isImageEmpty(kernel));
}

/**
 * @brief Tests that the method for morphological transformations does not throw an exception.
 */
TEST_F(OpenCvWrapperTest, morphologyTransformationsNoThrow)
{
    // Morphological transformations parameters
    const OpenCvWrapper::MorphTypes op{OpenCvWrapper::MorphTypes::MORPH_ERODE};
    const ImageMat kernel{mOpenCvWrapper->getStructuringElement(OpenCvWrapper::MorphShapes::MORPH_RECT, 3)};
    const unsigned int iterations{3};

    EXPECT_NO_THROW(mOpenCvWrapper->morphologyEx(mTestImage3chn, mTestImage3chn, op, kernel, iterations));
}

/**
 * @brief Tests that the method for finding contours does not throw an exception.
 */
TEST_F(OpenCvWrapperTest, findContoursNoThrow)
{
    // Find contours parameters
    Contours contours{};
    ContoursHierarchy hierarchy{};
    const OpenCvWrapper::RetrievalModes mode{OpenCvWrapper::RetrievalModes::RETR_LIST};
    const OpenCvWrapper::ContourApproximationModes method{
        OpenCvWrapper::ContourApproximationModes::CHAIN_APPROX_SIMPLE};

    EXPECT_NO_THROW(mOpenCvWrapper->findContours(mTestImage1chn, contours, hierarchy, mode, method));
}

/**
 * @brief Tests that the method for drawing contours does not throw an exception.
 */
TEST_F(OpenCvWrapperTest, drawContoursNoThrow)
{
    const auto filePath{cExistentImageFilePath};

    // Read image
    auto image = mOpenCvWrapper->readImage(filePath);

    // Image should be filtered before the contours finding
    mOpenCvWrapper->convertImageToGray(image, image);
    mOpenCvWrapper->gaussianBlurImage(image, image, 5);

    Contours contours{};
    ContoursHierarchy hierarchy{};

    // Find contours parameters
    const OpenCvWrapper::RetrievalModes mode{OpenCvWrapper::RetrievalModes::RETR_LIST};
    const OpenCvWrapper::ContourApproximationModes method{
        OpenCvWrapper::ContourApproximationModes::CHAIN_APPROX_SIMPLE};

    mOpenCvWrapper->findContours(image, contours, hierarchy, mode, method);

    // Draw contours parameters
    const int contourIdx{-1};
    const Scalar& color{255, 0, 0};
    const int thickness{1};
    const OpenCvWrapper::LineTypes lineType{OpenCvWrapper::LineTypes::LINE_8};

    EXPECT_NO_THROW(mOpenCvWrapper->drawContours(image, contours, contourIdx, color, thickness, lineType, hierarchy));
}

/**
 * @brief Tests that the contour area is calculated correctly.
 */
TEST_F(OpenCvWrapperTest, calculatesContourArea)
{
    std::vector<Point> contour{Point(0, 0), Point(10, 0), Point(10, 10), Point(0, 10)};

    EXPECT_EQ(100, static_cast<int>(mOpenCvWrapper->contourArea(contour)));
}

/**
 * @brief Tests that the methods for bounding rectangle and for drawing rectangle do not throw an exception.
 */
TEST_F(OpenCvWrapperTest, boundingAndDrawingRectNoThrow)
{
    const auto filePath{cExistentImageFilePath};

    // Read image
    auto image = mOpenCvWrapper->readImage(filePath);

    // Image should be filtered before the contours finding
    mOpenCvWrapper->convertImageToGray(image, image);
    mOpenCvWrapper->gaussianBlurImage(image, image, 5);

    Contours contours{};
    ContoursHierarchy hierarchy{};

    // Find contours parameters
    const OpenCvWrapper::RetrievalModes mode{OpenCvWrapper::RetrievalModes::RETR_LIST};
    const OpenCvWrapper::ContourApproximationModes method{
        OpenCvWrapper::ContourApproximationModes::CHAIN_APPROX_SIMPLE};

    mOpenCvWrapper->findContours(image, contours, hierarchy, mode, method);

    // Rectangle parameters
    Rectangle rect{};
    const Scalar& color{255, 0, 0};
    const int thickness{1};
    const OpenCvWrapper::LineTypes lineType{OpenCvWrapper::LineTypes::LINE_8};

    for (const auto contour : contours) {
        EXPECT_NO_THROW(rect = mOpenCvWrapper->boundingRect(contour));
        EXPECT_NO_THROW(mOpenCvWrapper->rectangle(image, rect, color, thickness, lineType));
    }
}

/**
 * @brief Tests if the rectangle dimensions (width and height) and coordinates (x and y) are correct.
 */
TEST_F(OpenCvWrapperTest, checksRectangleDimensionsCoordinates)
{
    constexpr unsigned int x{5};
    constexpr unsigned int y{10};
    constexpr unsigned int width{20};
    constexpr unsigned int height{30};
    Rectangle rect{x, y, width, height};

    EXPECT_EQ(x, mOpenCvWrapper->getRectCoordX(rect));
    EXPECT_EQ(y, mOpenCvWrapper->getRectCoordY(rect));
    EXPECT_EQ(width, mOpenCvWrapper->getRectWidth(rect));
    EXPECT_EQ(height, mOpenCvWrapper->getRectHeight(rect));
}

/**
 * @brief Tests if the rectangle creation is done correctly.
 */
TEST_F(OpenCvWrapperTest, createsRectangle)
{
    constexpr unsigned int x{5};
    constexpr unsigned int y{10};
    constexpr unsigned int width{20};
    constexpr unsigned int height{30};
    Rectangle rect{mOpenCvWrapper->createRect(x, y, width, height)};

    EXPECT_EQ(x, mOpenCvWrapper->getRectCoordX(rect));
    EXPECT_EQ(y, mOpenCvWrapper->getRectCoordY(rect));
    EXPECT_EQ(width, mOpenCvWrapper->getRectWidth(rect));
    EXPECT_EQ(height, mOpenCvWrapper->getRectHeight(rect));
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