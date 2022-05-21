/**
 * @file
 */

#include "ImagePreprocessing.h"
#include <opencv2/imgproc.hpp>

using namespace circuitSegmentation::imageProcessing;

ImagePreprocessing::ImagePreprocessing(std::shared_ptr<ImageProcUtils> imageProcUtils,
                                       std::shared_ptr<logging::Logger> logger,
                                       bool saveImages)
    : mImageProcUtils{std::move(imageProcUtils)}
    , mLogger{std::move(logger)}
    , mSaveImages{std::move(saveImages)}
{
}

void ImagePreprocessing::preprocessImage(cv::Mat& image)
{
    mLogger->logInfo("Starting image preprocessing");

    // Resize image
    resizeImage(image);

    // Save image
    if (mSaveImages) {
        mImageProcUtils->writeImage("image_resize.png", image);
        // TODO: Remove or comment.
        mImageProcUtils->showImage("Resized image", image, 0);
    }

    // Convert to grayscale
    convertImageToGray(image);

    // Save image
    if (mSaveImages) {
        mImageProcUtils->writeImage("image_grayscale.png", image);
        // TODO: Remove or comment.
        mImageProcUtils->showImage("Converted image to grayscale", image, 0);
    }

    // Blur image
    blurImage(image);

    // Save image
    if (mSaveImages) {
        mImageProcUtils->writeImage("image_blur.png", image);
        // TODO: Remove or comment.
        mImageProcUtils->showImage("Blurred image", image, 0);
    }

    // Detect edges
    edgesImage(image);

    // Save image
    if (mSaveImages) {
        mImageProcUtils->writeImage("image_edge_detection.png", image);
        // TODO: Remove or comment.
        mImageProcUtils->showImage("Edge detection image", image, 0);
    }
}

void ImagePreprocessing::resizeImage(cv::Mat& image)
{
    /*
     * Resize image
     * - Set the size of the image to work with, using a scaling factor
     * - The aspect ratio of the image is preserved
     */

    // Image width
    const auto widthImg = image.size().width;
    // Image height
    const auto heightImg = image.size().height;

    mLogger->logInfo("Initial image size: width = " + std::to_string(widthImg)
                     + ", height = " + std::to_string(heightImg));

    // Resize scaling factor to be used for width and height
    double resizeScale{1};
    // Flag to do the resize operation
    auto doResize{false};

    // Resize the image if any dimension (width or height) is larger than the maximum
    if (widthImg >= heightImg && widthImg > cResizeDim) {
        resizeScale = cResizeDim / static_cast<double>(widthImg);
        doResize = true;
    } else if (heightImg >= widthImg && heightImg > cResizeDim) {
        resizeScale = cResizeDim / static_cast<double>(heightImg);
        doResize = true;
    }

    // Resize
    if (doResize) {
        cv::resize(image, image, cv::Size(), resizeScale, resizeScale, cv::InterpolationFlags::INTER_LINEAR);

        mLogger->logInfo("Resize scale = " + std::to_string(resizeScale));
        mLogger->logInfo("Image resized: width = " + std::to_string(image.size().width)
                         + ", height = " + std::to_string(image.size().height));
    }
}

void ImagePreprocessing::convertImageToGray(cv::Mat& image)
{
    /*
     * Convert to grayscale
     * - Reduce the complexity of the calculations
     * - In this application, the color information is not relevant
     */
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    mLogger->logInfo("Image converted to grayscale");
}

void ImagePreprocessing::blurImage(cv::Mat& image)
{
    /*
     * Gaussian blurring
     * - Reduce noise
     * - Improve edge detection
     */
    cv::GaussianBlur(image, image, cv::Size(cFilterKernelSize, cFilterKernelSize), 0);

    mLogger->logInfo("Gaussian blurring applied to the image");
}

void ImagePreprocessing::edgesImage(cv::Mat& image)
{
    /*
     * Detect edges using the Canny Edge Detector
     * - The Canny Edge Detector algorithm follows a 3-stage process:
     *      - Calculation of the intensity gradient of the image
     *      - Suppression of false edges
     *      - Hysteresis thresholding
     * - Before applying the Canny Edge Detector, the image should be filtered (using Gaussian blurring, for example)
     * - Canny recommended a ratio of high:low threshold between 2:1 and 3:1
     */
    cv::Canny(image, image, cCannyEdgeThresh1, cCannyEdgeThresh2, cCannyEdgeApertureSize);

    mLogger->logInfo("Canny edge detector applied to the image");
}

void ImagePreprocessing::setSaveImages(const bool& saveImages)
{
    mSaveImages = saveImages;
}

bool ImagePreprocessing::getSaveImages() const
{
    return mSaveImages;
}
