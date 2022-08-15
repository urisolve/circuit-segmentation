/**
 * @file
 */

#pragma once

#include "circuit/Component.h"
#include "circuit/Connection.h"
#include "circuit/Node.h"
#include "circuit/Position.h"
#include "computerVision/OpenCvWrapper.h"
#include "logging/Logger.h"
#include <memory>
#include <vector>

namespace circuitSegmentation {
namespace schematicSegmentation {

/**
 * @brief Schematic segmentation.
 *
 * This class has the elements segmented from the circuit image.
 */
class SchematicSegmentation
{
public:
    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit SchematicSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                   const std::shared_ptr<logging::Logger>& logger);

    /**
     * @brief Destructor.
     */
    virtual ~SchematicSegmentation() = default;

    /**
     * @brief Detects connection points (ports) of components.
     *
     * @param imageInitial Initial image without preprocessing.
     * @param imagePreprocessed Image preprocessed for segmentation.
     * @param componentsDetected Components detected.
     * @param connectionsDetected Connections detected.
     * @param nodesDetected Nodes detected.
     * @param saveImages Save images obtained during the processing.
     */
    virtual void detectComponentConnections(computerVision::ImageMat& imageInitial,
                                            computerVision::ImageMat& imagePreprocessed,
                                            const std::vector<circuit::Component>& componentsDetected,
                                            const std::vector<circuit::Connection>& connectionsDetected,
                                            const std::vector<circuit::Node>& nodesDetected,
                                            const bool saveImages = false);

    /**
     * @brief Updates detected components of the circuit.
     *
     * This method verifies that each detected component has, at least, a connection. If there are no connections
     * to a component, that component is discarded from the list of detected components.
     *
     * @return True if there are components detected, otherwise false.
     */
    virtual bool updateDetectedComponents();

    /**
     * @brief Associates labels to the elements of the circuit.
     *
     * @param imageInitial Initial image without preprocessing.
     * @param imagePreprocessed Image preprocessed for segmentation.
     * @param labelsDetected Labels detected.
     * @param saveImages Save images obtained during the processing.
     */
    virtual void associateLabels(computerVision::ImageMat& imageInitial,
                                 computerVision::ImageMat& imagePreprocessed,
                                 const std::vector<circuit::Label>& labelsDetected,
                                 const bool saveImages = false);

    /**
     * @brief Gets the segmented components.
     *
     * @return Segmented components.
     */
    [[nodiscard]] virtual const std::vector<circuit::Component>& getComponents() const;

    /**
     * @brief Gets the segmented connections.
     *
     * @return Segmented connections.
     */
    [[nodiscard]] virtual const std::vector<circuit::Connection>& getConnections() const;

    /**
     * @brief Gets the segmented nodes.
     *
     * @return Segmented nodes.
     */
    [[nodiscard]] virtual const std::vector<circuit::Node>& getNodes() const;

    /**
     * @brief Gets the segmented labels.
     *
     * @return Segmented labels.
     */
    [[nodiscard]] virtual const std::vector<circuit::Label>& getLabels() const;

#ifndef BUILD_TESTS
private:
#endif
    /**
     * @brief Calculates relative position for component port.
     *
     * @param connectionPoint Connection point with component.
     * @param box Component bounding box.
     * @param widthIncr Increment on bounding box width.
     * @param heightIncr Increment on bounding box height.
     *
     * @return Relative position for component port.
     */
    virtual circuit::RelativePosition calcPortPosition(const computerVision::Point& connectionPoint,
                                                       const computerVision::Rectangle& box,
                                                       const int widthIncr,
                                                       const int heightIncr);

private:
    /** Port contour color. */
    const computerVision::Scalar cPortColor{0, 0, 255};
    /** Port contour thickness. */
    const int cPortThickness{10};

    /** Bounding box color. */
    const computerVision::Scalar cBoxColor{0, 255, 0};
    /** Bounding box thickness. */
    const int cBoxThickness{2};

    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Components segmented. */
    std::vector<circuit::Component> mComponents;

    /** Connections segmented. */
    std::vector<circuit::Connection> mConnections;

    /** Nodes segmented. */
    std::vector<circuit::Node> mNodes;

    /** Labels segmented. */
    std::vector<circuit::Label> mLabels;
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
