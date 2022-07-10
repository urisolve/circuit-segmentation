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

double OpenCvWrapper::arcLength(InputOutputArray& curve, const bool& closed)
{
    return cv::arcLength(curve, closed);
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

void OpenCvWrapper::thinning(ImageMat& srcImg, ImageMat& dstImg, const ThinningAlgorithms& thinningAlg)
{
    ImageMat processed = srcImg.clone();
    processed /= 255;

    cv::Mat prev = cv::Mat::zeros(processed.size(), CV_8UC1);
    cv::Mat diff;

    do {
        thinningIter(processed, 0, thinningAlg);
        thinningIter(processed, 1, thinningAlg);
        cv::absdiff(processed, prev, diff);
        processed.copyTo(prev);
    } while (cv::countNonZero(diff) > 0);

    processed *= 255;

    dstImg = processed;
}

void OpenCvWrapper::thinningIter(ImageMat& img,
                                 const int& iter,
                                 const ThinningAlgorithms& thinningAlg = ThinningAlgorithms::THINNING_ZHANGSUEN)
{
    cv::Mat marker = cv::Mat::zeros(img.size(), CV_8UC1);

    if (thinningAlg == ThinningAlgorithms::THINNING_ZHANGSUEN) {
        for (int i = 1; i < img.rows - 1; i++) {
            for (int j = 1; j < img.cols - 1; j++) {
                uchar p2 = img.at<uchar>(i - 1, j);
                uchar p3 = img.at<uchar>(i - 1, j + 1);
                uchar p4 = img.at<uchar>(i, j + 1);
                uchar p5 = img.at<uchar>(i + 1, j + 1);
                uchar p6 = img.at<uchar>(i + 1, j);
                uchar p7 = img.at<uchar>(i + 1, j - 1);
                uchar p8 = img.at<uchar>(i, j - 1);
                uchar p9 = img.at<uchar>(i - 1, j - 1);

                int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) + (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1)
                        + (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) + (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
                int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
                int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
                int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

                if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                    marker.at<uchar>(i, j) = 1;
            }
        }
    }
    if (thinningAlg == ThinningAlgorithms::THINNING_GUOHALL) {
        for (int i = 1; i < img.rows - 1; i++) {
            for (int j = 1; j < img.cols - 1; j++) {
                uchar p2 = img.at<uchar>(i - 1, j);
                uchar p3 = img.at<uchar>(i - 1, j + 1);
                uchar p4 = img.at<uchar>(i, j + 1);
                uchar p5 = img.at<uchar>(i + 1, j + 1);
                uchar p6 = img.at<uchar>(i + 1, j);
                uchar p7 = img.at<uchar>(i + 1, j - 1);
                uchar p8 = img.at<uchar>(i, j - 1);
                uchar p9 = img.at<uchar>(i - 1, j - 1);

                int C = ((!p2) & (p3 | p4)) + ((!p4) & (p5 | p6)) + ((!p6) & (p7 | p8)) + ((!p8) & (p9 | p2));
                int N1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
                int N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);
                int N = N1 < N2 ? N1 : N2;
                int m = iter == 0 ? ((p6 | p7 | (!p9)) & p8) : ((p2 | p3 | (!p5)) & p4);

                if ((C == 1) && ((N >= 2) && ((N <= 3)) & (m == 0)))
                    marker.at<uchar>(i, j) = 1;
            }
        }
    }

    img &= ~marker;
}

} // namespace computerVision
} // namespace circuitSegmentation
