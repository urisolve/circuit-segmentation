/**
 * @file
 */

#include "ImagePreprocessing.h"

using namespace circuitSegmentation::imageProcessing;

ImagePreprocessing::ImagePreprocessing(std::shared_ptr<computerVision::OpenCvWrapper> openCvWrapper,
                                       std::shared_ptr<logging::Logger> logger,
                                       bool saveImages)
    : mOpenCvWrapper{std::move(openCvWrapper)}
    , mLogger{std::move(logger)}
    , mSaveImages{std::move(saveImages)}
{
}

void ImagePreprocessing::preprocessImage(computerVision::ImageMat& image)
{
    mLogger->logInfo("Starting image preprocessing");

    // Resize image
    resizeImage(image);

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_preproc_1_resize.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Resized image", image, 0);
    }

    // Convert to grayscale
    convertImageToGray(image);

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_preproc_2_grayscale.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Converted image to grayscale", image, 0);
    }

    // Blur image
    blurImage(image);

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_preproc_3_blur.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Blurred image", image, 0);
    }

    // Detect edges
    edgesImage(image);

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_preproc_4_edge_detect.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Edge detection image", image, 0);
    }
}

void ImagePreprocessing::resizeImage(computerVision::ImageMat& image)
{
    /*
     * Resize image
     * - Set the size of the image to work with, using a scaling factor
     * - The aspect ratio of the image is preserved
     */

    // Image width
    const auto widthImg = mOpenCvWrapper->getImageWidth(image);
    // Image height
    const auto heightImg = mOpenCvWrapper->getImageHeight(image);

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
        mOpenCvWrapper->resizeImage(image, image, resizeScale);

        mLogger->logInfo("Resize scale = " + std::to_string(resizeScale));
        mLogger->logInfo("Image resized: width = " + std::to_string(mOpenCvWrapper->getImageWidth(image))
                         + ", height = " + std::to_string(mOpenCvWrapper->getImageHeight(image)));
    }
}

void ImagePreprocessing::convertImageToGray(computerVision::ImageMat& image)
{
    /*
     * Convert to grayscale
     * - Reduce the complexity of the calculations
     * - In this application, the color information is not relevant
     */
    mOpenCvWrapper->convertImageToGray(image, image);

    mLogger->logInfo("Image converted to grayscale");
}

void ImagePreprocessing::blurImage(computerVision::ImageMat& image)
{
    /*
     * Gaussian blurring
     * - Reduce noise
     * - Improve edge detection
     */
    mOpenCvWrapper->gaussianBlurImage(image, image, cFilterKernelSize);

    mLogger->logInfo("Gaussian blurring applied to the image");
}

void ImagePreprocessing::edgesImage(computerVision::ImageMat& image)
{
    // Detect edges using the Canny Edge Detector
    mOpenCvWrapper->cannyEdgeImage(image, image, cCannyEdgeThresh1, cCannyEdgeThresh2, cCannyEdgeApertureSize);

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
