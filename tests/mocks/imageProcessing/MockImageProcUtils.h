/**
 * @file
 */

#pragma once

#include "imageProcessing/ImageProcUtils.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace imageProcessing {

/**
 * @brief Mock of the ImageProcUtils class.
 */
class MockImageProcUtils : public ImageProcUtils
{
public:
    /** Mocks method "virtual void showImage(const std::string& windowName, cv::Mat& image, int delay)". */
    MOCK_METHOD(void, showImage, (const std::string& windowName, cv::Mat& image, int delay), (override));
    /** Mocks method "virtual bool writeImage(const std::string& fileName, cv::Mat& image)". */
    MOCK_METHOD(bool, writeImage, (const std::string& fileName, cv::Mat& image), (override));
};

} // namespace imageProcessing
} // namespace circuitSegmentation
