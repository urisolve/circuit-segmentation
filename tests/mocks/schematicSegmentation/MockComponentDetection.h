/**
 * @file
 */

#pragma once

#include "schematicSegmentation/ComponentDetection.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Mock of the ComponentDetection class.
 */
class MockComponentDetection : public ComponentDetection
{
public:
    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit MockComponentDetection(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                    const std::shared_ptr<logging::Logger>& logger)
        : ComponentDetection(openCvWrapper, logger)
    {
    }

    /** Mocks method detectComponents. */
    MOCK_METHOD(bool, detectComponents, (computerVision::ImageMat&, computerVision::ImageMat&, const bool), (override));
    /** Mocks method getDetectedComponents. */
    MOCK_METHOD(const std::vector<circuit::Component>&, getDetectedComponents, (), (const, override));
    /** Mocks method boundingBoxComponents. */
    MOCK_METHOD(void, boundingBoxComponents, (const computerVision::Contours&, computerVision::ImageMat&), (override));
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
