/**
 * @file
 */

#pragma once

#include "imageProcessing/ImageSegmentation.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace imageProcessing {

/**
 * @brief Mock of the ImageSegmentation class.
 */
class MockImageSegmentation : public ImageSegmentation
{
public:
    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     * @param componentSegmentation Component segmentation.
     * @param connectionSegmentation Connection segmentation.
     * @param saveImages Save images obtained during the processing.
     */
    explicit MockImageSegmentation(
        const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
        const std::shared_ptr<logging::Logger>& logger,
        const std::shared_ptr<schematicSegmentation::ComponentSegmentation>& componentSegmentation,
        const std::shared_ptr<schematicSegmentation::ConnectionSegmentation>& connectionSegmentation,
        const bool saveImages = false)
        : ImageSegmentation(openCvWrapper, logger, componentSegmentation, connectionSegmentation, saveImages)
    {
    }

    /** Mocks method segmentImage. */
    MOCK_METHOD(bool, segmentImage, (computerVision::ImageMat, computerVision::ImageMat), (override));
    /** Mocks method setSaveImages. */
    MOCK_METHOD(void, setSaveImages, (const bool&), (override));
    /** Mocks method getSaveImages. */
    MOCK_METHOD(bool, getSaveImages, (), (const, override));
};

} // namespace imageProcessing
} // namespace circuitSegmentation
