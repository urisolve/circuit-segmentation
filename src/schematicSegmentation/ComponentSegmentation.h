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
 * @brief Component segmentation.
 */
class ComponentSegmentation
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
    explicit ComponentSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                   const std::shared_ptr<logging::Logger>& logger);

    /**
     * @brief Destructor.
     */
    virtual ~ComponentSegmentation() = default;

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
     * @brief Detects connection points (ports) of components.
     *
     * @param imagePreprocessed Image preprocessed for segmentation.
     * @param connections Connections detected.
     */
    virtual void detectComponentConnections(computerVision::ImageMat& imagePreprocessed,
                                            const std::vector<circuit::Connection>& connections);

    /**
     * @brief Updates detected components of the circuit.
     *
     * This method verifies that each detected component is connected to a connection. If there are no connections
     * connected to a component, that component is discarded from the list of detected components.
     *
     * @return True if there are components detected, otherwise false.
     */
    virtual bool updateDetectedComponents();

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

    /**
     * @brief Increases size of bounding box and centers it.
     *
     * @param box Bounding box.
     * @param widthIncr Increment for width.
     * @param heightIncr Increment for height.
     * @param widthMax Maximum width (x coordinate + box width).
     * @param heightMax Maximum height (y coordinate + box height).
     *
     * @return Enlarged size bounding box.
     */
    virtual computerVision::Rectangle increaseBoundingBox(const computerVision::Rectangle& box,
                                                          const int& widthIncr,
                                                          const int& heightIncr,
                                                          const int& widthMax,
                                                          const int& heightMax);

#ifdef BUILD_TESTS
public:
    /**
     * @brief Sets the detected components.
     *
     * This method is only useful for testing purposes.
     *
     * @param components Detected components.
     */
    virtual void setDetectedComponents(const std::vector<circuit::Component>& components);
#endif

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
    const int cBoundingBoxThickness{3};
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
