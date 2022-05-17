/**
 * @file
 */

#include "ImageProcUtils.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace circuitSegmentation::imageProcessing;

void ImageProcUtils::showImage(const std::string& windowName, cv::Mat& image, int delay)
{
    // Open image
    cv::imshow(windowName, image);
    // Wait for a pressed key
    cv::waitKey(delay);
}

bool ImageProcUtils::writeImage(const std::string& fileName, cv::Mat& image)
{
    // Result of the operation
    auto result{false};

    try {
        // Write image
        result = cv::imwrite(fileName, image);
    }
    catch (const cv::Exception& ex) {
        result = false;
    }

    return result;
}
