/**
 * @file
 */

#pragma once

#include <opencv2/core.hpp>
#include <string>

namespace circuitSegmentation {
namespace computerVision {

/** Alias for image matrix. */
using ImageMat = cv::Mat;
/** Alias for contours. */
using Contours = std::vector<std::vector<cv::Point>>;
/** Alias for hierarchy for finding contours. */
using ContoursHierarchy = std::vector<cv::Vec4i>;
/** Alias for scalar. */
using Scalar = cv::Scalar;
/** Alias for rectangle. */
using Rectangle = cv::Rect;
/** Alias for input/output array. */
using InputOutputArray = cv::InputOutputArray;
/** Alias for point. */
using Point = cv::Point;

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
     * @brief Enumeration of the shapes of the structuring element.
     *
     * @note The values are mapped to MorphShapes of the OpenCV library.
     */
    enum class MorphShapes : unsigned char {
        /** Mapped to MORPH_RECT of the OpenCV library. */
        MORPH_RECT = 0,
        /** Mapped to MORPH_CROSS of the OpenCV library. */
        MORPH_CROSS = 1,
        /** Mapped to MORPH_ELLIPSE of the OpenCV library. */
        MORPH_ELLIPSE = 2
    };

    /**
     * @brief Enumeration of the morphological operations.
     *
     * @note The values are mapped to MorphShapes of the OpenCV library.
     */
    enum class MorphTypes : unsigned char {
        /** Mapped to MORPH_ERODE of the OpenCV library. */
        MORPH_ERODE = 0,
        /** Mapped to MORPH_DILATE of the OpenCV library. */
        MORPH_DILATE = 1,
        /** Mapped to MORPH_OPEN of the OpenCV library. */
        MORPH_OPEN = 2,
        /** Mapped to MORPH_CLOSE of the OpenCV library. */
        MORPH_CLOSE = 3,
        /** Mapped to MORPH_GRADIENT of the OpenCV library. */
        MORPH_GRADIENT = 4,
        /** Mapped to MORPH_TOPHAT of the OpenCV library. */
        MORPH_TOPHAT = 5,
        /** Mapped to MORPH_BLACKHAT of the OpenCV library. */
        MORPH_BLACKHAT = 6,
        /** Mapped to MORPH_HITMISS of the OpenCV library. */
        MORPH_HITMISS = 7
    };

    /**
     * @brief Enumeration of the modes of the contour retrieval algorithm.
     *
     * @note The values are mapped to RetrievalModes of the OpenCV library.
     */
    enum class RetrievalModes : unsigned char {
        /** Mapped to RETR_EXTERNAL of the OpenCV library. */
        RETR_EXTERNAL = 0,
        /** Mapped to RETR_LIST of the OpenCV library. */
        RETR_LIST = 1,
        /** Mapped to RETR_CCOMP of the OpenCV library. */
        RETR_CCOMP = 2,
        /** Mapped to RETR_TREE of the OpenCV library. */
        RETR_TREE = 3,
        /** Mapped to RETR_FLOODFILL of the OpenCV library. */
        RETR_FLOODFILL = 4
    };

    /**
     * @brief Enumeration of the contour approximation algorithms.
     *
     * @note The values are mapped to ContourApproximationModes of the OpenCV library.
     */
    enum class ContourApproximationModes : unsigned char {
        /** Mapped to CHAIN_APPROX_NONE of the OpenCV library. */
        CHAIN_APPROX_NONE = 1,
        /** Mapped to CHAIN_APPROX_SIMPLE of the OpenCV library. */
        CHAIN_APPROX_SIMPLE = 2,
        /** Mapped to CHAIN_APPROX_TC89_L1 of the OpenCV library. */
        CHAIN_APPROX_TC89_L1 = 3,
        /** Mapped to CHAIN_APPROX_TC89_KCOS of the OpenCV library. */
        CHAIN_APPROX_TC89_KCOS = 4
    };

    /**
     * @brief Enumeration of the types of line.
     *
     * @note The values are mapped to LineTypes of the OpenCV library.
     */
    enum LineTypes {
        /** Mapped to FILLED of the OpenCV library. */
        FILLED = -1,
        /** Mapped to LINE_4 of the OpenCV library. */
        LINE_4 = 4,
        /** Mapped to LINE_8 of the OpenCV library. */
        LINE_8 = 8,
        /** Mapped to LINE_AA of the OpenCV library. */
        LINE_AA = 16
    };

    /**
     * @brief Constructor.
     */
    OpenCvWrapper() = default; // GCOVR_EXCL_LINE

    /**
     * @brief Destructor.
     */
    virtual ~OpenCvWrapper() = default; // GCOVR_EXCL_LINE

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

    /**
     * @brief Gets a structuring element of the specified size and shape for morphological operations.
     *
     * @param shape Element shape.
     * @param size Size of the structuring element.
     * @return Structuring element.
     */
    virtual ImageMat getStructuringElement(const MorphShapes& shape, const unsigned int& size);

    /**
     * @brief Performs advanced morphological transformations.
     *
     * @param srcImg Source image. The number of channels can be arbitrary.
     * @param dstImg Destination image of the same size and type as source image.
     * @param op Type of a morphological operation.
     * @param kernel Structuring element.
     * @param iterations Number of times erosion and dilation are applied.
     */
    virtual void morphologyEx(ImageMat& srcImg,
                              ImageMat& dstImg,
                              const MorphTypes& op,
                              const ImageMat& kernel,
                              const unsigned int& iterations);

    /**
     * @brief Finds contours in a binary image.
     *
     * @param image 8-bit input image.
     * @param contours Output of detected contours.
     * @param hierarchy Output of information about the image topology. It has as many elements as the number of
     * contours.
     * @param mode Contour retrieval mode.
     * @param method Contour approximation method.
     */
    virtual void findContours(ImageMat& image,
                              Contours& contours,
                              ContoursHierarchy& hierarchy,
                              const RetrievalModes& mode,
                              const ContourApproximationModes& method);

    /**
     * @brief Draws contours outlines or filled contours.
     *
     * This method draws contours outlines in the image if @p thickness >= 0, or fills the area bounded by the contours
     * if @p thickness < 0.
     *
     * @param image Destination image.
     * @param contours All the input contours.
     * @param contourIdx Index of the contour to draw. If it is negative, all the contours are drawn.
     * @param color Color of the contours.
     * @param thickness Thickness of lines the contours are drawn with. If it is negative, the contour interiors are
     * drawn.
     * @param lineType Line connectivity.
     * @param hierarchy Optional information about hierarchy. It is only needed if you want to draw only some of the
     * contours.
     */
    virtual void drawContours(ImageMat& image,
                              const Contours& contours,
                              const int& contourIdx,
                              const Scalar& color,
                              const int& thickness,
                              const LineTypes& lineType,
                              const ContoursHierarchy& hierarchy);

    /**
     * @brief Calculates a contour area.
     *
     * @param contour Input vector of 2D points (contour vertices).
     * @return Contour area.
     */
    virtual double contourArea(InputOutputArray& contour);

    /**
     * @brief Calculates the up-right bounding rectangle of a point set or non-zero pixels of gray-scale image.
     *
     * @param array Input gray-scale image or 2D point set.
     * @return Bounding rectangle.
     */
    virtual Rectangle boundingRect(InputOutputArray& array);

    /**
     * @brief Draws a simple, thick, or filled up-right rectangle.
     *
     * This method draws a rectangle outline or a filled rectangle whose two opposite corners are @p pt1 and @p pt2.
     *
     * @param image Destination image.
     * @param rectangle Rectangle to draw.
     * @param color Rectangle color or brightness (grayscale image).
     * @param thickness Thickness of lines that make up the rectangle. Negative values means that the method has to draw
     * a filled rectangle.
     * @param lineType Type of the line.
     */
    virtual void rectangle(ImageMat& image,
                           const Rectangle& rectangle,
                           const Scalar& color,
                           const int& thickness,
                           const LineTypes& lineType);

    /**
     * @brief Gets the width of a rectangle.
     *
     * @param rectangle Rectangle.
     *
     * @return Width of the rectangle.
     */
    [[nodiscard]] virtual int getRectWidth(Rectangle& rectangle) const;

    /**
     * @brief Gets the height of a rectangle.
     *
     * @param rectangle Rectangle.
     *
     * @return Height of the rectangle.
     */
    [[nodiscard]] virtual int getRectHeight(Rectangle& rectangle) const;

    /**
     * @brief Gets the x coordinate of the top-left corner of a rectangle.
     *
     * @param rectangle Rectangle.
     *
     * @return Value of x coordinate of the top-left corner of the rectangle.
     */
    [[nodiscard]] virtual int getRectCoordX(Rectangle& rectangle) const;

    /**
     * @brief Gets the y coordinate of the top-left corner of a rectangle.
     *
     * @param rectangle Rectangle.
     *
     * @return Value of y coordinate of the top-left corner of the rectangle.
     */
    [[nodiscard]] virtual int getRectCoordY(Rectangle& rectangle) const;

    /**
     * @brief Creates a rectangle.
     *
     * @param x Value of x coordinate of the top-left corner of the rectangle.
     * @param y Value of y coordinate of the top-left corner of the rectangle.
     * @param width Wdith of the rectangle.
     * @param height Height of the rectangle.
     *
     * @return Created rectangle.
     */
    [[nodiscard]] virtual Rectangle createRect(const int& x, const int& y, const int& width, const int& height) const;
};

} // namespace computerVision
} // namespace circuitSegmentation
