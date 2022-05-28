/**
 * @file
 */

#pragma once

#include <opencv2/core.hpp>
#include <string>

namespace circuitSegmentation {
namespace computerVision {

/** Alias to image matrix. */
using ImageMat = cv::Mat;

/**
 * @brief Wrapper of the OpenCV library.
 */
class OpenCvWrapper
{
public:
    /**
     * @brief Constructor.
     */
    OpenCvWrapper() = default;

    /**
     * @brief Destructor.
     */
    virtual ~OpenCvWrapper() = default;

    /**
     * @brief Sets the log mode.
     *
     * @param silent Silent mode.
     */
    virtual void setLogMode(const bool& silent);

    /**
     * @brief Gets the log mode.
     *
     * @return True if silent mode, otherwise false.
     */
    [[nodiscard]] virtual bool getLogMode() const;

    /**
     * @brief Shows the image in a new window.
     *
     * @param windowName Window name.
     * @param image Image.
     * @param delay Delay to wait for a pressed key, in milliseconds. The method waits for a key event infinitely (when
     * delay <= 0), or for the specified delay when it is positive.
     */
    virtual void showImage(const std::string& windowName, ImageMat& image, int delay);

    /**
     * @brief Writes the image to the file.
     *
     * @param fileName File name.
     * @param image Image.
     *
     * @return True if the operation occurred successfully, otherwise false.
     */
    virtual bool writeImage(const std::string& fileName, ImageMat& image);

    /**
     * @brief Reads the image from file.
     *
     * @param fileName File name.
     *
     * @return Image read from the specified file, or if the image cannot be read (because of missing file, improper
     * permissions, unsupported or invalid format), an empty matrix.
     */
    virtual ImageMat readImage(const std::string& fileName);

    /**
     * @brief Clones an image.
     *
     * @param image Image.
     *
     * @return Cloned image.
     */
    virtual ImageMat cloneImage(ImageMat& image);

    /**
     * @brief Checks if an image is empty.
     *
     * @param image Image.
     *
     * @return True if image is empty, otherwise false.
     */
    [[nodiscard]] virtual bool isImageEmpty(ImageMat& image);

    /**
     * @brief Resizes an image.
     *
     * @param srcImg Input image.
     * @param dstImg Output image.
     * @param scale Scale factor.
     */
    virtual void resizeImage(ImageMat& srcImg, ImageMat& dstImg, const double& scale);

    /**
     * @brief Gets the width of an image.
     *
     * @param image Image.
     *
     * @return Width of the image.
     */
    [[nodiscard]] virtual int getImageWidth(ImageMat& image) const;

    /**
     * @brief Gets the height of an image.
     *
     * @param image Image.
     *
     * @return Height of the image.
     */
    [[nodiscard]] virtual int getImageHeight(ImageMat& image) const;

    /**
     * @brief Converts an image to grayscale.
     *
     * @param srcImg Input image.
     * @param dstImg Output image.
     */
    virtual void convertImageToGray(ImageMat& srcImg, ImageMat& dstImg);

    /**
     * @brief Blurs an image using a Gaussian filter.
     *
     * @param srcImg Input image.
     * @param dstImg Output image.
     * @param kernelSize Gaussian kernel size. The value must be positive and odd.
     */
    virtual void gaussianBlurImage(ImageMat& srcImg, ImageMat& dstImg, const unsigned int& kernelSize);

    /**
     * @brief Finds edges in an image using the Canny algorithm.
     *
     * The Canny Edge Detector algorithm follows a 3-stage process:
     * - Calculation of the intensity gradient of the image
     * - Suppression of false edges
     * - Hysteresis thresholding
     *
     * Before applying the Canny Edge Detector, the image should be filtered (using Gaussian blurring, for example).
     *
     * Canny recommended a ratio of high:low threshold between 2:1 and 3:1.
     *
     * @param srcImg 8-bit input image.
     * @param dstImg 8-bit output image.
     * @param threshold1 First threshold for the hysteresis procedure.
     * @param threshold2 Second threshold for the hysteresis procedure.
     * @param apertureSize Aperture size for the Sobel operator.
     */
    virtual void cannyEdgeImage(ImageMat& srcImg,
                                ImageMat& dstImg,
                                const double& threshold1,
                                const double& threshold2,
                                const int& apertureSize);
};

} // namespace computerVision
} // namespace circuitSegmentation
