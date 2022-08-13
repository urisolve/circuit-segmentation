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
 * @brief Component detection.
 */
class ComponentDetection
{
public:
    /** Minimum area for bounding boxes. */
    static constexpr int cBoxMinArea{300};

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
     * @param connections Connections detected.
     * @param saveImages Save images obtained during the processing.
     *
     * @return True if there are components detected, otherwise false.
     */
    virtual bool detectComponents(computerVision::ImageMat& imageInitial,
                                  computerVision::ImageMat& imagePreprocessed,
                                  const std::vector<circuit::Connection>& connections,
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
     * @brief Removes the connections from image (set connections with black pixels).
     *
     * @param image Image to remove connections.
     * @param connections Connections.
     */
    virtual void removeConnectionsFromImage(computerVision::ImageMat& image,
                                            const std::vector<circuit::Connection>& connections);

    /**
     * @brief Check if the contour has the minimum area and intersection points with connections.
     *
     * @param imagePreprocessed Image preprocessed for segmentation.
     * @param contour Contour.
     * @param connections Connections.
     *
     * @return Bounding box for the contour if the contour has the minimum area and intersection points with
     * connections, otherwise a null optional.
     */
    virtual std::optional<computerVision::Rectangle> checkContour(computerVision::ImageMat& imagePreprocessed,
                                                                  const computerVision::Contour& contour,
                                                                  const std::vector<circuit::Connection>& connections);

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
    const unsigned int cMorphCloseKernelSize{7};
    /** Iterations for morphological closing. */
    const unsigned int cMorphCloseIter{3};

    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Components detected. */
    std::vector<circuit::Component> mComponents;
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
