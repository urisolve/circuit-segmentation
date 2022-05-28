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
     * @brief Enumeration of the adaptive threshold algorithms.
     *
     * @note The values are mapped to AdaptiveThresholdTypes of the OpenCV library.
     */
    enum class AdaptiveThresholdAlgorithm : unsigned char {
        /** Mapped to ADAPTIVE_THRESH_MEAN_C of the OpenCV library. */
        ADAPTIVE_THRESH_MEAN = 0,
        /** Mapped to ADAPTIVE_THRESH_GAUSSIAN_C of the OpenCV library. */
        ADAPTIVE_THRESH_GAUSSIAN = 1
    };

    /**
     * @brief Enumeration of the threshold operations.
     *
     * @note The values are mapped to ThresholdTypes of the OpenCV library.
     */
    enum class ThresholdOperations : unsigned char {
        /** Mapped to THRESH_BINARY of the OpenCV library. */
        THRESH_BINARY = 0,
        /** Mapped to THRESH_BINARY_INV of the OpenCV library. */
        THRESH_BINARY_INV = 1,
        /** Mapped to THRESH_TRUNC of the OpenCV library. */
        THRESH_TRUNC = 2,
        /** Mapped to THRESH_TOZERO of the OpenCV library. */
        THRESH_TOZERO = 3,
        /** Mapped to THRESH_TOZERO_INV of the OpenCV library. */
        THRESH_TOZERO_INV = 4,
        /** Mapped to THRESH_MASK of the OpenCV library. */
        THRESH_MASK = 7,
        /** Mapped to THRESH_OTSU of the OpenCV library. */
        THRESH_OTSU = 8,
        /** Mapped to THRESH_TRIANGLE of the OpenCV library. */
        THRESH_TRIANGLE = 16
    };

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
     * @brief Applies an adaptive threshold to an image.
     *
     * Before applying the adaptive threshold, the image should be filtered (using Gaussian blurring, for example).
     *
     * @param srcImg 8-bit input image.
     * @param dstImg 8-bit output image.
     * @param maxValue Non-zero value assigned to the pixels for which the condition is satisfied.
     * @param adaptiveMethod Adaptive thresholding algorithm to use.
     * @param thresholdOp Threshold operation type to use.
     * @param blockSize Size of a pixel neighborhood that is used to calculate a threshold value for the pixel: 3, 5, 7,
     * and so on.
     * @param subConst Constant subtracted from the mean or weighted mean. Normally, it is positive but may be zero or
     * negative as well.
     */
    virtual void adaptiveThresholdImage(ImageMat& srcImg,
                                        ImageMat& dstImg,
                                        const double& maxValue,
                                        const AdaptiveThresholdAlgorithm& adaptiveMethod,
                                        const ThresholdOperations& thresholdOp,
                                        const int& blockSize,
                                        const double& subConst);

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
