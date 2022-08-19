/**
 * @file
 */

#pragma once

#include "schematicSegmentation/SegmentationMap.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Mock of the SegmentationMap class.
 */
class MockSegmentationMap : public SegmentationMap
{
public:
    /**
     * @brief Constructor.
     *
     * @param logger Logger.
     */
    explicit MockSegmentationMap(const std::shared_ptr<logging::Logger>& logger)
        : SegmentationMap(logger)
    {
    }

    /** Mocks method generateSegmentationMap. */
    MOCK_METHOD(bool,
                generateSegmentationMap,
                (const std::vector<circuit::Component>&,
                 const std::vector<circuit::Connection>&,
                 const std::vector<circuit::Node>&),
                (override));
    /** Mocks method writeSegmentationMapJsonFile. */
    MOCK_METHOD(bool, writeSegmentationMapJsonFile, (), (override));
    /** Mocks method getSegmentationMap. */
    MOCK_METHOD(const nlohmann::ordered_json&, getSegmentationMap, (), (override, const));
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
