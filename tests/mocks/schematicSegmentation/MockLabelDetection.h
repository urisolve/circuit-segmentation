/**
 * @file
 */

#pragma once

#include "schematicSegmentation/LabelDetection.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Mock of the LabelDetection class.
 */
class MockLabelDetection : public LabelDetection
{
public:
    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit MockLabelDetection(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                const std::shared_ptr<logging::Logger>& logger)
        : LabelDetection(openCvWrapper, logger)
    {
    }

    /** Mocks method detectLabels. */
    MOCK_METHOD(bool,
                detectLabels,
                (computerVision::ImageMat&,
                 computerVision::ImageMat&,
                 const std::vector<circuit::Component>&,
                 const std::vector<circuit::Connection>&,
                 const bool),
                (override));
    /** Mocks method getDetectedLabels. */
    MOCK_METHOD(const std::vector<circuit::Label>&, getDetectedLabels, (), (const, override));
    /** Mocks method removeElementsFromImage. */
    MOCK_METHOD(void,
                removeElementsFromImage,
                (computerVision::ImageMat&,
                 const std::vector<circuit::Component>&,
                 const std::vector<circuit::Connection>&),
                (override));
    /** Mocks method checkContour. */
    MOCK_METHOD(std::optional<computerVision::Rectangle>,
                checkContour,
                (computerVision::ImageMat&, const computerVision::Contour&),
                (override));
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
