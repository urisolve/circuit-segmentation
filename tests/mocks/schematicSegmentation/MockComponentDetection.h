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
    MOCK_METHOD(
        bool,
        detectComponents,
        (computerVision::ImageMat&, computerVision::ImageMat&, const std::vector<circuit::Connection>&, const bool),
        (override));
    /** Mocks method getDetectedComponents. */
    MOCK_METHOD(const std::vector<circuit::Component>&, getDetectedComponents, (), (const, override));
    /** Mocks method removeConnectionsFromImage. */
    MOCK_METHOD(void,
                removeConnectionsFromImage,
                (computerVision::ImageMat&, const std::vector<circuit::Connection>&),
                (override));
    /** Mocks method checkContour. */
    MOCK_METHOD(std::optional<computerVision::Rectangle>,
                checkContour,
                (computerVision::ImageMat&, const computerVision::Contour&, const std::vector<circuit::Connection>&),
                (override));
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
