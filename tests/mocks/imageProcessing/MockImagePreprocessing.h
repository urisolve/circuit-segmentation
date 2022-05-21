/**
 * @file
 */

#pragma once

#include "imageProcessing/ImagePreprocessing.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace imageProcessing {

/**
 * @brief Mock of the ImagePreprocessing class.
 */
class MockImagePreprocessing : public ImagePreprocessing
{
public:
    /** Mocks method "virtual void preprocessImage(cv::Mat& image)". */
    MOCK_METHOD(void, preprocessImage, (cv::Mat & image), (override));
    /** Mocks method "virtual void resizeImage(cv::Mat& image)". */
    MOCK_METHOD(void, resizeImage, (cv::Mat & image), (override));
    /** Mocks method "virtual void convertImageToGray(cv::Mat& image)". */
    MOCK_METHOD(void, convertImageToGray, (cv::Mat & image), (override));
    /** Mocks method "virtual void blurImage(cv::Mat& image)". */
    MOCK_METHOD(void, blurImage, (cv::Mat & image), (override));
    /** Mocks method "virtual void edgesImage(cv::Mat& image)". */
    MOCK_METHOD(void, edgesImage, (cv::Mat & image), (override));
    /** Mocks method "virtual void setSaveImages(bool& saveImages)". */
    MOCK_METHOD(void, setSaveImages, (bool& saveImages), (override));
    /** Mocks method "virtual bool getSaveImages() const". */
    MOCK_METHOD(bool, getSaveImages, (), (override, const));
};

} // namespace imageProcessing
} // namespace circuitSegmentation
