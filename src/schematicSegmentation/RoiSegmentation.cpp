/**
 * @file
 */

#include "RoiSegmentation.h"

namespace circuitSegmentation {
namespace schematicSegmentation {

RoiSegmentation::RoiSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                 const std::shared_ptr<logging::Logger>& logger)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
{
}

bool RoiSegmentation::generateRoiComponents(computerVision::ImageMat& imageInitial,
                                            const std::vector<circuit::Component>& components)
{
    mLogger->logInfo("Generating images with ROI for components");

    // Success for all images
    auto success{true};

    for (const auto& component : components) {
        // Get ROI
        const auto roi{component.mBoundingBox};

        // Generate image
        const std::string filePath{"roi_component_" + component.mId + ".png"};
        if (!generateRoi(imageInitial, roi, component.mId, filePath)) {
            success = false;
        }
    }

    return success;
}

bool RoiSegmentation::generateRoiLabels(computerVision::ImageMat& imageInitial,
                                        const std::vector<circuit::Component>& components,
                                        const std::vector<circuit::Connection>& connections,
                                        const std::vector<circuit::Node>& nodes)
{
    mLogger->logInfo("Generating images with ROI for labels");

    // Success for all images
    auto success{true};

    // Labels associated to components
    for (const auto& component : components) {
        // Get labels
        const auto labels{component.mLabels};

        for (auto it{labels.begin()}; it != labels.end(); ++it) {
            const auto index{it - labels.begin()};

            // Get ROI
            const auto roi{labels.at(index).mBoundingBox};

            // Generate image
            const std::string filePath{"roi_label_" + component.mId + "_" + std::to_string(index + 1) + ".png"};
            if (!generateRoi(imageInitial, roi, component.mId, filePath)) {
                success = false;
            }
        }
    }

    // Labels associated to connections
    for (const auto& connection : connections) {
        // Get labels
        const auto labels{connection.mLabels};

        for (auto it{labels.begin()}; it != labels.end(); ++it) {
            const auto index{it - labels.begin()};

            // Get ROI
            const auto roi{labels.at(index).mBoundingBox};

            // Generate image
            const std::string filePath{"roi_label_" + connection.mId + "_" + std::to_string(index + 1) + ".png"};
            if (!generateRoi(imageInitial, roi, connection.mId, filePath)) {
                success = false;
            }
        }
    }

    // Labels associated to nodes
    for (const auto& node : nodes) {
        // Get labels
        const auto labels{node.mLabels};

        for (auto it{labels.begin()}; it != labels.end(); ++it) {
            const auto index{it - labels.begin()};

            // Get ROI
            const auto roi{labels.at(index).mBoundingBox};

            // Generate image
            const std::string filePath{"roi_label_" + node.mId + "_" + std::to_string(index + 1) + ".png"};
            if (!generateRoi(imageInitial, roi, node.mId, filePath)) {
                success = false;
            }
        }
    }

    return success;
}

bool RoiSegmentation::generateRoi(computerVision::ImageMat& imageInitial,
                                  const computerVision::Rectangle& roi,
                                  const std::string& elementId,
                                  const std::string& filePath)
{
    // Crop image
    computerVision::ImageMat image{};
    if (!mOpenCvWrapper->cropImage(imageInitial, image, roi)) {
        mLogger->logError("Failed to crop image with ROI for element " + elementId);
        return false;
    }

    // Save image
    if (!mOpenCvWrapper->writeImage(filePath, image)) {
        mLogger->logError("Failed to write image with ROI for element " + elementId);
        return false;
    }

    return true;
}

} // namespace schematicSegmentation
} // namespace circuitSegmentation
