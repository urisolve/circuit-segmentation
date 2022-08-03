/**
 * @file
 */

#pragma once

#include "schematicSegmentation/SchematicSegmentation.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Mock of the SchematicSegmentation class.
 */
class MockSchematicSegmentation : public SchematicSegmentation
{
public:
    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit MockSchematicSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                       const std::shared_ptr<logging::Logger>& logger)
        : SchematicSegmentation(openCvWrapper, logger)
    {
    }

    /** Mocks method detectComponentConnections. */
    MOCK_METHOD(void,
                detectComponentConnections,
                (computerVision::ImageMat&,
                 computerVision::ImageMat&,
                 const std::vector<circuit::Component>&,
                 const std::vector<circuit::Connection>&,
                 const std::vector<circuit::Node>&,
                 const bool),
                (override));
    /** Mocks method updateDetectedComponents. */
    MOCK_METHOD(bool, updateDetectedComponents, (), (override));
    /** Mocks method getComponents. */
    MOCK_METHOD(const std::vector<circuit::Component>&, getComponents, (), (const, override));
    /** Mocks method getConnections. */
    MOCK_METHOD(const std::vector<circuit::Connection>&, getConnections, (), (const, override));
    /** Mocks method getNodes. */
    MOCK_METHOD(const std::vector<circuit::Node>&, getNodes, (), (const, override));
    /** Mocks method calcPortPosition. */
    MOCK_METHOD(circuit::RelativePosition,
                calcPortPosition,
                (const computerVision::Point&, const computerVision::Rectangle&, const int, const int),
                (override));
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
