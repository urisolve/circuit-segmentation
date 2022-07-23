/**
 * @file
 */

#pragma once

#include "circuit/Component.h"
#include "circuit/Connection.h"
#include "computerVision/OpenCvWrapper.h"
#include "logging/Logger.h"
#include <memory>
#include <vector>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Component detection.
 */
class ComponentDetection
{
public:
    /** Bounding box minimum area. */
    static constexpr double cBoundingBoxMinArea{200};

    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit ComponentDetection(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                const std::shared_ptr<logging::Logger>& logger);

    /**
     * @brief Destructor.
     */
    virtual ~ComponentDetection() = default;

    /**
     * @brief Detects the components of the circuit.
     *
     * @param imageInitial Initial image without preprocessing.
     * @param imagePreprocessed Image preprocessed for segmentation.
     * @param saveImages Save images obtained during the processing.
     *
     * @return True if there are components detected, otherwise false.
     */
    virtual bool detectComponents(computerVision::ImageMat& imageInitial,
                                  computerVision::ImageMat& imagePreprocessed,
                                  const bool saveImages = false);

    /**
     * @brief Gets the detected components.
     *
     * @return Detected components.
     */
    [[nodiscard]] virtual const std::vector<circuit::Component>& getDetectedComponents() const;

#ifndef BUILD_TESTS
private:
#endif
    /**
     * @brief Generates bounding boxes for the circuit components.
     *
     * @param blobs Components represented as blobs.
     * @param imagePreprocessed Image preprocessed for segmentation.
     */
    virtual void
        boundingBoxComponents(const computerVision::Contours& blobs, computerVision::ImageMat& imagePreprocessed);

private:
    /** Size of the kernel for morphological closing. */
    const unsigned int cMorphCloseKernelSize{11};
    /** Iterations for morphological closing. */
    const unsigned int cMorphCloseIter{4};

    /** Size of the kernel for morphological opening. */
    const unsigned int cMorphOpenKernelSize{3};
    /** Iterations for morphological opening. */
    const unsigned int cMorphOpenIter{1};

    /** Mode of contour retrieval algorithm for bounding boxes. */
    const computerVision::OpenCvWrapper::RetrievalModes cBoundingBoxFindContourMode{
        computerVision::OpenCvWrapper::RetrievalModes::RETR_EXTERNAL};
    /** Contour approximation algorithm for bounding boxes. */
    const computerVision::OpenCvWrapper::ContourApproximationModes cBoundingBoxFindContourMethod{
        computerVision::OpenCvWrapper::ContourApproximationModes::CHAIN_APPROX_SIMPLE};
    /** Bounding boxes color. */
    const computerVision::Scalar cBoundingBoxColor{0, 255, 0};
    /** Bounding boxes thickness. */
    const int cBoundingBoxThickness{2};
    /** Width to increase the bounding box. */
    const int cBoundingBoxWidthIncrease{20};
    /** Height to increase the bounding box. */
    const int cBoundingBoxHeightIncrease{20};

    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Components detected. */
    std::vector<circuit::Component> mComponents;
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
