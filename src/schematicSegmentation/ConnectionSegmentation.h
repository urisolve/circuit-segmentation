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
 * @brief Connection segmentation.
 */
class ConnectionSegmentation
{
public:
    /** Connection minimum length. */
    static constexpr double cConnectionMinLength{20};

    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit ConnectionSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                    const std::shared_ptr<logging::Logger>& logger);

    /**
     * @brief Destructor.
     */
    virtual ~ConnectionSegmentation() = default;

    /**
     * @brief Detects the connections of the circuit.
     *
     * @param imageInitial Initial image without preprocessing.
     * @param imagePreprocessed Image preprocessed for segmentation.
     * @param components Components detected.
     * @param saveImages Save images obtained during the processing.
     *
     * @return True if there are connections detected, otherwise false.
     */
    virtual bool detectConnections(computerVision::ImageMat& imageInitial,
                                   computerVision::ImageMat& imagePreprocessed,
                                   const std::vector<circuit::Component>& components,
                                   const bool saveImages = false);

    /**
     * @brief Gets the detected connections.
     *
     * @return Detected connections.
     */
    [[nodiscard]] virtual const std::vector<circuit::Connection>& getDetectedConnections() const;

private:
    /** Mode of contour retrieval algorithm for connections detection. */
    const computerVision::OpenCvWrapper::RetrievalModes cConnectionFindContourMode{
        computerVision::OpenCvWrapper::RetrievalModes::RETR_EXTERNAL};
    /** Contour approximation algorithm for connections detection. */
    const computerVision::OpenCvWrapper::ContourApproximationModes cConnectionFindContourMethod{
        computerVision::OpenCvWrapper::ContourApproximationModes::CHAIN_APPROX_SIMPLE};
    /** Connection contour color. */
    const computerVision::Scalar cConnectionColor{0, 0, 255};
    /** Connection contour thickness. */
    const int cConnectionThickness{3};

    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Connections detected. */
    std::vector<circuit::Connection> mConnections;
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
