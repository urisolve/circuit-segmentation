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
 * @brief Class responsible for generation of images with ROI (regions of interest) from image segmentation.
 */
class RoiSegmentation
{
public:
    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit RoiSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                             const std::shared_ptr<logging::Logger>& logger);

    /**
     * @brief Destructor.
     */
    virtual ~RoiSegmentation() = default;

    /**
     * @brief Generates images with ROI for components.
     *
     * @param imageInitial Initial image without preprocessing.
     * @param components Components.
     *
     * @note The image files are written to the working directory and have the following format for naming:
     * "roi_component_\<component_id\>.png".
     *
     * @return True if all images generation occurred successfully, otherwise false.
     */
    virtual bool generateRoiComponents(computerVision::ImageMat& imageInitial,
                                       const std::vector<circuit::Component>& components);

    /**
     * @brief Generates images with ROI for labels associated to circuit elements.
     *
     * @param imageInitial Initial image without preprocessing.
     * @param components Components.
     * @param connections Connections.
     * @param nodes Nodes.
     *
     * @note The image files are written to the working directory and have the following format for naming:
     * "roi_label_\<associated_element_id\>_\<n\>.png". Meaning of fields:
     * - associated_element_id: ID of the associated element to this label. Note that this is not the ID of the label.
     * - n: label number. As the element can have more than one label associated, this just specifies a number for the
     * label.
     *
     * @return True if all images generation occurred successfully, otherwise false.
     */
    virtual bool generateRoiLabels(computerVision::ImageMat& imageInitial,
                                   const std::vector<circuit::Component>& components,
                                   const std::vector<circuit::Connection>& connections,
                                   const std::vector<circuit::Node>& nodes);

private:
    /**
     * @brief Generates image with ROI.
     *
     * @param imageInitial Initial image without preprocessing.
     * @param roi ROI.
     * @param elementId Circuit element ID.
     * @param filePath File path to save image.
     *
     * @return True if image generation occurred successfully, otherwise false.
     */
    virtual bool generateRoi(computerVision::ImageMat& imageInitial,
                             const computerVision::Rectangle& roi,
                             const std::string& elementId,
                             const std::string& filePath);

private:
    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
};

} // namespace schematicSegmentation
} // namespace circuitSegmentation
