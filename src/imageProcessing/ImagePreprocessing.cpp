/**
 * @file
 */

#include "ImagePreprocessing.h"

namespace circuitSegmentation {
namespace imageProcessing {

ImagePreprocessing::ImagePreprocessing(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                       const std::shared_ptr<logging::Logger>& logger,
                                       const bool saveImages)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
    , mSaveImages{std::move(saveImages)}
{
}

void ImagePreprocessing::preprocessImage(computerVision::ImageMat& image)
{
    mLogger->logInfo("Starting image preprocessing");

    // Convert to grayscale
    convertImageToGray(image);

    // Blur image
    blurImage(image);

    // Apply threshold
    thresholdImage(image);

    // Apply morphological dilation
    morphologicalDilateImage(image);

    // Apply thinning operation
    thinningImage(image);
}

void ImagePreprocessing::setSaveImages(const bool& saveImages)
{
    mSaveImages = saveImages;
}

bool ImagePreprocessing::getSaveImages() const
{
    return mSaveImages;
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

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_preproc_grayscale.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Converted image to grayscale", image, 0);
    }
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

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_preproc_blur.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Blurred image", image, 0);
    }
}

void ImagePreprocessing::thresholdImage(computerVision::ImageMat& image)
{
    /*
     * Adaptive threshold
     * - Consider small neighbors of pixels and then find an optimal threshold value, T, for each neighbor
     * - Tend to produce good results, but is more computationally expensive than Otsu’s method or simple thresholding
     * - Very useful in cases where:
     *      - We have non-uniform illumination conditions
     *      - There may be dramatic ranges of pixel intensities and the optimal value of T may change for different
     * parts of the image
     */
    mOpenCvWrapper->adaptiveThresholdImage(
        image, image, cThresholdMaxValue, cThresholdMethod, cThresholdOp, cThresholdBlockSize, cThresholdSubConst);

    mLogger->logInfo("Adaptive threshold applied to the image");

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_preproc_threshold.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Thresholding image", image, 0);
    }
}

void ImagePreprocessing::morphologicalOpenImage(computerVision::ImageMat& image)
{
    /*
     * Morphological opening
     * - Performs an opening operation that allows us to remove small blobs from an image
     * - First an erosion is applied to remove the small blobs, then a dilation is applied to regrow the size of the
     * original object
     */
    const auto kernelMorph = mOpenCvWrapper->getStructuringElement(
        circuitSegmentation::computerVision::OpenCvWrapper::MorphShapes::MORPH_RECT, cMorphOpenKernelSize);
    mOpenCvWrapper->morphologyEx(image,
                                 image,
                                 circuitSegmentation::computerVision::OpenCvWrapper::MorphTypes::MORPH_OPEN,
                                 kernelMorph,
                                 cMorphOpenIter);

    mLogger->logInfo("Morphological opening applied to the image");

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_preproc_morph_open.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological opening image", image, 0);
    }
}

void ImagePreprocessing::morphologicalDilateImage(computerVision::ImageMat& image)
{
    /*
     * Morphological dilation
     * - Opposite of an erosion
     * - Grows the foreground pixels, increasing the size of foreground objects
     * - Useful for joining broken parts of an image together
     */
    const auto kernelMorph = mOpenCvWrapper->getStructuringElement(
        circuitSegmentation::computerVision::OpenCvWrapper::MorphShapes::MORPH_RECT, cMorphDilateKernelSize);
    mOpenCvWrapper->morphologyEx(image,
                                 image,
                                 circuitSegmentation::computerVision::OpenCvWrapper::MorphTypes::MORPH_DILATE,
                                 kernelMorph,
                                 cMorphDilateIter);

    mLogger->logInfo("Morphological dilation applied to the image");

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_preproc_morph_dilation.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological dilation image", image, 0);
    }
}

void ImagePreprocessing::thinningImage(computerVision::ImageMat& image)
{
    /*
     * Thinning operation
     * - Transformation of a digital image into a simplified, but topologically equivalent image
     * - Allows to obtain the image skeleton, with each line thickness transformed into a pixel
     * - This is useful to be independent of the contours thickness in the initial image
     */
    mOpenCvWrapper->thinning(
        image, image, circuitSegmentation::computerVision::OpenCvWrapper::ThinningAlgorithms::THINNING_ZHANGSUEN);

    mLogger->logInfo("Thinning operation applied to the image");

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_preproc_thinning.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Thinning image", image, 0);
    }
}

void ImagePreprocessing::edgesImage(computerVision::ImageMat& image)
{
    // Detect edges using the Canny Edge Detector
    mOpenCvWrapper->cannyEdgeImage(image, image, cCannyEdgeThresh1, cCannyEdgeThresh2, cCannyEdgeApertureSize);

    mLogger->logInfo("Canny edge detector applied to the image");
}

} // namespace imageProcessing
} // namespace circuitSegmentation
