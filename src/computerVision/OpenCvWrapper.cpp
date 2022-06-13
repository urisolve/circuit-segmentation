/**
 * @file
 */

#include "OpenCvWrapper.h"
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace circuitSegmentation {
namespace computerVision {

void OpenCvWrapper::setLogMode(const bool& silent)
{
    if (silent) {
        // Set OpenCV log level to Silent
        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    } else {
        // Set OpenCV log level to Warning
        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
    }
}

bool OpenCvWrapper::getLogMode() const
{
    if (cv::utils::logging::getLogLevel() == cv::utils::logging::LOG_LEVEL_SILENT) {
        return true;
    } else {
        return false;
    }
}

// GCOVR_EXCL_START
// Rationale: It is not worth to test this logic.
void OpenCvWrapper::showImage(const std::string& windowName, ImageMat& image, int delay)
{
    // Open image
    cv::imshow(windowName, image);
    // Wait for a pressed key
    cv::waitKey(std::move(delay));
}
// GCOVR_EXCL_STOP

bool OpenCvWrapper::writeImage(const std::string& fileName, ImageMat& image)
{
    // Result of the operation
    auto result{false};

    try {
        // Write image
        result = cv::imwrite(fileName, image);
    }
    catch ([[maybe_unused]] const cv::Exception& ex) {
        result = false;
    }

    return result;
}

ImageMat OpenCvWrapper::readImage(const std::string& fileName)
{
    // Read image
    ImageMat image = cv::imread(fileName, cv::IMREAD_COLOR);
    return image;
}

ImageMat OpenCvWrapper::cloneImage(ImageMat& image)
{
    return image.clone();
}

bool OpenCvWrapper::isImageEmpty(ImageMat& image)
{
    return image.empty();
}

void OpenCvWrapper::resizeImage(ImageMat& srcImg, ImageMat& dstImg, const double& scale)
{
    // Resize image
    cv::resize(srcImg, dstImg, cv::Size(), scale, scale, cv::InterpolationFlags::INTER_LINEAR);
}

int OpenCvWrapper::getImageWidth(ImageMat& image) const
{
    return image.size().width;
}

int OpenCvWrapper::getImageHeight(ImageMat& image) const
{
    return image.size().height;
}

void OpenCvWrapper::convertImageToGray(ImageMat& srcImg, ImageMat& dstImg)
{
    // Convert to grayscale
    cv::cvtColor(srcImg, dstImg, cv::COLOR_BGR2GRAY);
}

void OpenCvWrapper::gaussianBlurImage(ImageMat& srcImg, ImageMat& dstImg, const unsigned int& kernelSize)
{
    // Gaussian blurring
    cv::GaussianBlur(srcImg, dstImg, cv::Size(kernelSize, kernelSize), 0);
}

void OpenCvWrapper::adaptiveThresholdImage(ImageMat& srcImg,
                                           ImageMat& dstImg,
                                           const double& maxValue,
                                           const AdaptiveThresholdAlgorithm& adaptiveMethod,
                                           const ThresholdOperations& thresholdOp,
                                           const int& blockSize,
                                           const double& subConst)
{
    // Adaptive threshold
    cv::adaptiveThreshold(
        srcImg, dstImg, maxValue, static_cast<int>(adaptiveMethod), static_cast<int>(thresholdOp), blockSize, subConst);
}

void OpenCvWrapper::cannyEdgeImage(
    ImageMat& srcImg, ImageMat& dstImg, const double& threshold1, const double& threshold2, const int& apertureSize)
{
    // Detect edges using the Canny Edge Detector
    cv::Canny(srcImg, dstImg, threshold1, threshold2, apertureSize);
}

ImageMat OpenCvWrapper::getStructuringElement(const MorphShapes& shape, const unsigned int& size)
{
    return cv::getStructuringElement(static_cast<int>(shape), cv::Size(size, size), cv::Point(-1, -1));
}

void OpenCvWrapper::morphologyEx(
    ImageMat& srcImg, ImageMat& dstImg, const MorphTypes& op, const ImageMat& kernel, const unsigned int& iterations)
{
    cv::morphologyEx(srcImg, dstImg, static_cast<int>(op), kernel, cv::Point(-1, -1), iterations);
}

void OpenCvWrapper::findContours(ImageMat& image,
                                 Contours& contours,
                                 ContoursHierarchy& hierarchy,
                                 const RetrievalModes& mode,
                                 const ContourApproximationModes& method)
{
    cv::findContours(image, contours, hierarchy, static_cast<int>(mode), static_cast<int>(method));
}

void OpenCvWrapper::drawContours(ImageMat& image,
                                 const Contours& contours,
                                 const int& contourIdx,
                                 const Scalar& color,
                                 const int& thickness,
                                 const LineTypes& lineType,
                                 const ContoursHierarchy& hierarchy)
{
    cv::drawContours(image, contours, contourIdx, color, thickness, lineType, hierarchy);
}

double OpenCvWrapper::contourArea(InputOutputArray& contour)
{
    return cv::contourArea(contour);
}

Rectangle OpenCvWrapper::boundingRect(InputOutputArray& array)
{
    return cv::boundingRect(array);
}

void OpenCvWrapper::rectangle(
    ImageMat& image, const Rectangle& rectangle, const Scalar& color, const int& thickness, const LineTypes& lineType)
{
    cv::rectangle(image, rectangle, color, thickness, lineType);
}

bool OpenCvWrapper::contains(const Rectangle& rectangle, const Point& point)
{
    return rectangle.contains(point);
}

} // namespace computerVision
} // namespace circuitSegmentation
