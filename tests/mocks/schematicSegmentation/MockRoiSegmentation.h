/**
 * @file
 */

#pragma once

#include "schematicSegmentation/RoiSegmentation.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Mock of the RoiSegmentation class.
 */
class MockRoiSegmentation : public RoiSegmentation
{
public:
    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit MockRoiSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                 const std::shared_ptr<logging::Logger>& logger)
        : RoiSegmentation(openCvWrapper, logger)
    {
    }

    /** Mocks method generateRoiComponents. */
    MOCK_METHOD(bool,
                generateRoiComponents,
                (computerVision::ImageMat&, const std::vector<circuit::Component>&),
                (override));
    /** Mocks method generateRoiLabels. */
    MOCK_METHOD(bool,
                generateRoiLabels,
                (computerVision::ImageMat&,
                 const std::vector<circuit::Component>&,
                 const std::vector<circuit::Connection>&,
                 const std::vector<circuit::Node>&),
                (override));
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
