/**
 * @file
 */

#pragma once

#include "circuit/Component.h"
#include "circuit/Connection.h"
#include "circuit/Node.h"
#include "computerVision/OpenCvWrapper.h"
#include "logging/Logger.h"
#include <memory>
#include <vector>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Connection detection.
 */
class ConnectionDetection
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
    explicit ConnectionDetection(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                 const std::shared_ptr<logging::Logger>& logger);

    /**
     * @brief Destructor.
     */
    virtual ~ConnectionDetection() = default;

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
     * @brief Detects the nodes of the circuit, and updates connections.
     *
     * @param imageInitial Initial image without preprocessing.
     * @param imagePreprocessed Image preprocessed for segmentation.
     * @param components Components detected.
     * @param saveImages Save images obtained during the processing.
     *
     * @return True if there are nodes and/or connections detected, otherwise false.
     */
    virtual bool detectNodesUpdateConnections(computerVision::ImageMat& imageInitial,
                                              computerVision::ImageMat& imagePreprocessed,
                                              const std::vector<circuit::Component>& components,
                                              const bool saveImages = false);

    /**
     * @brief Gets the detected connections.
     *
     * @return Detected connections.
     */
    [[nodiscard]] virtual const std::vector<circuit::Connection>& getDetectedConnections() const;

    /**
     * @brief Gets the detected nodes.
     *
     * @return Detected nodes.
     */
    [[nodiscard]] virtual const std::vector<circuit::Node>& getDetectedNodes() const;

#ifdef BUILD_TESTS
public:
    /**
     * @brief Sets the detected connections.
     *
     * @param connections Detected connections.
     */
    void setDetectedConnections(const std::vector<circuit::Connection>& connections);
#endif

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
    const int cConnectionThickness{2};

    /** Node contour color. */
    const computerVision::Scalar cNodeColor{255, 0, 0};
    /** Node contour thickness. */
    const int cNodeThickness{10};

    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Connections detected. */
    std::vector<circuit::Connection> mConnections;
    /** Nodes detected. */
    std::vector<circuit::Node> mNodes;
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
