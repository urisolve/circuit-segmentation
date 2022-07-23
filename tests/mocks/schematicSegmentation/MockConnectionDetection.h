/**
 * @file
 */

#pragma once

#include "schematicSegmentation/ConnectionDetection.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Mock of the ConnectionDetection class.
 */
class MockConnectionDetection : public ConnectionDetection
{
public:
    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit MockConnectionDetection(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                     const std::shared_ptr<logging::Logger>& logger)
        : ConnectionDetection(openCvWrapper, logger)
    {
    }

    /** Mocks method detectConnections. */
    MOCK_METHOD(
        bool,
        detectConnections,
        (computerVision::ImageMat&, computerVision::ImageMat&, const std::vector<circuit::Component>&, const bool),
        (override));
    /** Mocks method detectNodesUpdateConnections. */
    MOCK_METHOD(
        bool,
        detectNodesUpdateConnections,
        (computerVision::ImageMat&, computerVision::ImageMat&, const std::vector<circuit::Component>&, const bool),
        (override));
    /** Mocks method getDetectedConnections. */
    MOCK_METHOD(const std::vector<circuit::Connection>&, getDetectedConnections, (), (const, override));
    /** Mocks method getDetectedNodes. */
    MOCK_METHOD(const std::vector<circuit::Node>&, getDetectedNodes, (), (const, override));
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
