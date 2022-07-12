/**
 * @file
 */

#include "ConnectionSegmentation.h"

namespace circuitSegmentation {
namespace schematicSegmentation {

using namespace circuitSegmentation::computerVision;

ConnectionSegmentation::ConnectionSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                               const std::shared_ptr<logging::Logger>& logger)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
    , mConnections{}
{
}

bool ConnectionSegmentation::detectConnections(computerVision::ImageMat& imageInitial,
                                               computerVision::ImageMat& imagePreprocessed,
                                               const std::vector<circuit::Component>& components,
                                               const bool saveImages)
{
    /*
     * Detection of connections
     * - Separation of components found and circuit (bounding boxes as black pixels), so the components are removed
     * - Contour finding algorithm is applied to identify each connection (wire = contour)
     */

    mLogger->logInfo("Detecting connections of the circuit");

    // Image used during the process
    computerVision::ImageMat image{};

    // Set bounding boxes as black pixels, so the components are removed
    image = mOpenCvWrapper->cloneImage(imagePreprocessed);
    for (const auto& component : components) {
        mOpenCvWrapper->rectangle(image, component.mBoundingBox, {0, 0, 0}, -1, OpenCvWrapper::LineTypes::LINE_8);
    }

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("image_segment_remove_components.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Remove components", image, 0);
    }

    // At this point, the connections are represented as wires in the image, so we need to find those wires

    Contours wires{};
    ContoursHierarchy hierarchy{};

    mOpenCvWrapper->findContours(image, wires, hierarchy, cConnectionFindContourMode, cConnectionFindContourMethod);

    mLogger->logDebug("Contours found in the image, to detect connections: " + std::to_string(wires.size()));

    // Wire for each connection

    mConnections.clear();

    for (const auto& wire : wires) {
        // Check wire length
        if (mOpenCvWrapper->arcLength(wire, false) >= cConnectionMinLength) {
            // Add connection
            circuit::Connection connection{};
            connection.mWire = wire;
            mConnections.push_back(connection);
        }
    }

    mLogger->logInfo("Connections found in the circuit: " + std::to_string(mConnections.size()));

    // If there are no connections detected, it makes no sense to continue
    if (mConnections.empty()) {
        return false;
    }

    // Save image
    if (saveImages) {
        image = mOpenCvWrapper->cloneImage(imageInitial);
        Contours wires{};
        for (const auto& connection : mConnections) {
            wires.push_back(connection.mWire);
        }
        mOpenCvWrapper->drawContours(
            image, wires, -1, cConnectionColor, cConnectionThickness, OpenCvWrapper::LineTypes::LINE_8, {});

        mOpenCvWrapper->writeImage("image_segment_detect_connections.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Detecting connections", image, 0);
    }

    return true;
}

const std::vector<circuit::Connection>& ConnectionSegmentation::getDetectedConnections() const
{
    return mConnections;
}

} // namespace schematicSegmentation
} // namespace circuitSegmentation
