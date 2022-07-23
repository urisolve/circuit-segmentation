/**
 * @file
 */

#pragma once

#include "schematicSegmentation/ComponentSegmentation.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Mock of the ComponentSegmentation class.
 */
class MockComponentSegmentation : public ComponentSegmentation
{
public:
    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit MockComponentSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                       const std::shared_ptr<logging::Logger>& logger)
        : ComponentSegmentation(openCvWrapper, logger)
    {
    }

    /** Mocks method detectComponents. */
    MOCK_METHOD(bool, detectComponents, (computerVision::ImageMat&, computerVision::ImageMat&, const bool), (override));
    /** Mocks method detectComponentConnections. */
    MOCK_METHOD(void,
                detectComponentConnections,
                (computerVision::ImageMat&, const std::vector<circuit::Connection>&),
                (override));
    /** Mocks method updateDetectedComponents. */
    MOCK_METHOD(bool, updateDetectedComponents, (), (override));
    /** Mocks method getDetectedComponents. */
    MOCK_METHOD(const std::vector<circuit::Component>&, getDetectedComponents, (), (const, override));
    /** Mocks method boundingBoxComponents. */
    MOCK_METHOD(void, boundingBoxComponents, (const computerVision::Contours&, computerVision::ImageMat&), (override));
    /** Mocks method increaseBoundingBox. */
    MOCK_METHOD(computerVision::Rectangle,
                increaseBoundingBox,
                (const computerVision::Rectangle&, const int&, const int&, const int&, const int&),
                (override));
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
