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
    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     * @param saveImages Save images obtained during the processing.
     */
    explicit MockImagePreprocessing(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                    const std::shared_ptr<logging::Logger>& logger,
                                    const bool saveImages = false)
        : ImagePreprocessing(openCvWrapper, logger, saveImages)
    {
    }

    /** Mocks method preprocessImage. */
    MOCK_METHOD(void, preprocessImage, (computerVision::ImageMat&), (override));
    /** Mocks method setSaveImages. */
    MOCK_METHOD(void, setSaveImages, (const bool&), (override));
    /** Mocks method getSaveImages. */
    MOCK_METHOD(bool, getSaveImages, (), (const, override));
    /** Mocks method resizeImage. */
    MOCK_METHOD(void, resizeImage, (computerVision::ImageMat&), (override));
    /** Mocks method convertImageToGray. */
    MOCK_METHOD(void, convertImageToGray, (computerVision::ImageMat&), (override));
    /** Mocks method blurImage. */
    MOCK_METHOD(void, blurImage, (computerVision::ImageMat&), (override));
    /** Mocks method thresholdImage. */
    MOCK_METHOD(void, thresholdImage, (computerVision::ImageMat&), (override));
    /** Mocks method morphologicalOpenImage. */
    MOCK_METHOD(void, morphologicalOpenImage, (computerVision::ImageMat&), (override));
    /** Mocks method morphologicalDilateImage. */
    MOCK_METHOD(void, morphologicalDilateImage, (computerVision::ImageMat&), (override));
    /** Mocks method thinningImage. */
    MOCK_METHOD(void, thinningImage, (computerVision::ImageMat&), (override));
    /** Mocks method edgesImage. */
    MOCK_METHOD(void, edgesImage, (computerVision::ImageMat&), (override));
};

} // namespace imageProcessing
} // namespace circuitSegmentation
