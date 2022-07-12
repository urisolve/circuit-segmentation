/**
 * @file
 */

#pragma once

#include "schematicSegmentation/ConnectionSegmentation.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Mock of the ConnectionSegmentation class.
 */
class MockConnectionSegmentation : public ConnectionSegmentation
{
public:
    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit MockConnectionSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                        const std::shared_ptr<logging::Logger>& logger)
        : ConnectionSegmentation(openCvWrapper, logger)
    {
    }

    /** Mocks method detectConnections. */
    MOCK_METHOD(
        bool,
        detectConnections,
        (computerVision::ImageMat&, computerVision::ImageMat&, const std::vector<circuit::Component>&, const bool),
        (override));
    /** Mocks method getDetectedConnections. */
    MOCK_METHOD(const std::vector<circuit::Connection>&, getDetectedConnections, (), (const, override));
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
