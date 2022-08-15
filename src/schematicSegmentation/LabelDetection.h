/**
 * @file
 */

#pragma once

#include "circuit/Component.h"
#include "circuit/Connection.h"
#include "computerVision/OpenCvWrapper.h"
#include "logging/Logger.h"
#include <memory>
#include <optional>
#include <vector>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Label detection.
 */
class LabelDetection
{
public:
    /** Minimum area for bounding boxes. */
    static constexpr int cBoxMinArea{50};

    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit LabelDetection(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                            const std::shared_ptr<logging::Logger>& logger);

    /**
     * @brief Destructor.
     */
    virtual ~LabelDetection() = default;

    /**
     * @brief Detects the labels of the circuit.
     *
     * @param imageInitial Initial image without preprocessing.
     * @param imagePreprocessed Image preprocessed for segmentation.
     * @param components Components detected.
     * @param connections Connections detected.
     * @param saveImages Save images obtained during the processing.
     *
     * @return True if there are labels detected, otherwise false.
     */
    virtual bool detectLabels(computerVision::ImageMat& imageInitial,
                              computerVision::ImageMat& imagePreprocessed,
                              const std::vector<circuit::Component>& components,
                              const std::vector<circuit::Connection>& connections,
                              const bool saveImages = false);

    /**
     * @brief Gets the detected labels.
     *
     * @return Detected labels.
     */
    [[nodiscard]] virtual const std::vector<circuit::Label>& getDetectedLabels() const;

#ifndef BUILD_TESTS
private:
#endif
    /**
     * @brief Removes the elements from image (set elements with black pixels).
     *
     * @param image Image to remove elements.
     * @param components Components.
     * @param connections Connections.
     */
    virtual void removeElementsFromImage(computerVision::ImageMat& image,
                                         const std::vector<circuit::Component>& components,
                                         const std::vector<circuit::Connection>& connections);

    /**
     * @brief Check if the contour has the minimum area.
     *
     * @param imagePreprocessed Image preprocessed for segmentation.
     * @param contour Contour.
     *
     * @return Bounding box for the contour if the contour has the minimum area, otherwise a null optional.
     */
    virtual std::optional<computerVision::Rectangle>
        checkContour(computerVision::ImageMat& imagePreprocessed, const computerVision::Contour& contour);

private:
    /** Mode of contour retrieval algorithm to find contours. */
    const computerVision::OpenCvWrapper::RetrievalModes cFindContourMode{
        computerVision::OpenCvWrapper::RetrievalModes::RETR_EXTERNAL};
    /** Contour approximation algorithm to find contours. */
    const computerVision::OpenCvWrapper::ContourApproximationModes cFindContourMethod{
        computerVision::OpenCvWrapper::ContourApproximationModes::CHAIN_APPROX_SIMPLE};

    /** Bounding box color. */
    const computerVision::Scalar cBoxColor{0, 255, 0};
    /** Bounding box thickness. */
    const int cBoxThickness{2};

    /** Size of the kernel for morphological closing. */
    const unsigned int cMorphCloseKernelSize{9};
    /** Iterations for morphological closing. */
    const unsigned int cMorphCloseIter{3};

    /** Size of the kernel for morphological opening. */
    const unsigned int cMorphOpenKernelSize{3};
    /** Iterations for morphological opening. */
    const unsigned int cMorphOpenIter{1};

    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Labels detected. */
    std::vector<circuit::Label> mLabels;
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
