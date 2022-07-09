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
     * @param saveImages Save images obtained during the processing.
     */
    explicit MockImageSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                   const std::shared_ptr<logging::Logger>& logger,
                                   const bool saveImages = false)
        : ImageSegmentation(openCvWrapper, logger, saveImages)
    {
    }

    /** Mocks method segmentImage. */
    MOCK_METHOD(bool,
                segmentImage,
                (computerVision::ImageMat imageInitial, computerVision::ImageMat imagePreprocessed),
                (override));
    /** Mocks method setSaveImages. */
    MOCK_METHOD(void, setSaveImages, (const bool& saveImages), (override));
    /** Mocks method getSaveImages. */
    MOCK_METHOD(bool, getSaveImages, (), (const, override));
    /** Mocks method detectComponents. */
    MOCK_METHOD(bool, detectComponents, (), (override));
    /** Mocks method boundingBoxComponents. */
    MOCK_METHOD(void, boundingBoxComponents, (const computerVision::Contours& blobs), (override));
    /** Mocks method increaseBoundingBox. */
    MOCK_METHOD(computerVision::Rectangle,
                increaseBoundingBox,
                (const computerVision::Rectangle& box,
                 const int& widthIncr,
                 const int& heightIncr,
                 const int& widthMax,
                 const int& heightMax),
                (override));
    /** Mocks method detectNodes. */
    MOCK_METHOD(bool, detectNodes, (), (override));
    /** Mocks method detectCircuitConnections. */
    MOCK_METHOD(void, detectCircuitConnections, (), (override));
    /** Mocks method updateDetectedComponents. */
    MOCK_METHOD(bool, updateDetectedComponents, (), (override));
    /** Mocks method getDetectedComponents. */
    MOCK_METHOD(std::vector<circuit::Component>, getDetectedComponents, (), (const, override));
};

} // namespace imageProcessing
} // namespace circuitSegmentation
