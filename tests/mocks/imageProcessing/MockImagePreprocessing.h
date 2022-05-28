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
    /** Mocks method preprocessImage. */
    MOCK_METHOD(void, preprocessImage, (computerVision::ImageMat & image), (override));
    /** Mocks method resizeImage. */
    MOCK_METHOD(void, resizeImage, (computerVision::ImageMat & image), (override));
    /** Mocks method convertImageToGray. */
    MOCK_METHOD(void, convertImageToGray, (computerVision::ImageMat & image), (override));
    /** Mocks method blurImage. */
    MOCK_METHOD(void, blurImage, (computerVision::ImageMat & image), (override));
    /** Mocks method thresholdImage. */
    MOCK_METHOD(void, thresholdImage, (computerVision::ImageMat & image), (override));
    /** Mocks method edgesImage. */
    MOCK_METHOD(void, edgesImage, (computerVision::ImageMat & image), (override));
    /** Mocks method setSaveImages. */
    MOCK_METHOD(void, setSaveImages, (const bool& saveImages), (override));
    /** Mocks method getSaveImages. */
    MOCK_METHOD(bool, getSaveImages, (), (const, override));
};

} // namespace imageProcessing
} // namespace circuitSegmentation
