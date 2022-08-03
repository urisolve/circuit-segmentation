/**
 * @file
 */

#include "ConnectionDetection.h"
#include "SegmentationUtils.h"

namespace circuitSegmentation {
namespace schematicSegmentation {

ConnectionDetection::ConnectionDetection(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                         const std::shared_ptr<logging::Logger>& logger)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
    , mConnections{}
{
}

bool ConnectionDetection::detectConnections(computerVision::ImageMat& imageInitial,
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
        mOpenCvWrapper->rectangle(
            image, component.mBoundingBox, {0, 0, 0}, -1, computerVision::OpenCvWrapper::LineTypes::LINE_8);
    }

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("image_segment_remove_components.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Remove components", image, 0);
    }

    // At this point, the connections are represented as wires in the image, so we need to find those wires

    computerVision::Contours wires{};
    computerVision::ContoursHierarchy hierarchy{};

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
        computerVision::Contours wires{};
        for (const auto& connection : mConnections) {
            wires.push_back(connection.mWire);
        }
        mOpenCvWrapper->drawContours(image,
                                     wires,
                                     -1,
                                     cConnectionColor,
                                     cConnectionThickness,
                                     computerVision::OpenCvWrapper::LineTypes::LINE_8,
                                     {});

        mOpenCvWrapper->writeImage("image_segment_detect_connections.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Detecting connections", image, 0);
    }

    return true;
}

bool ConnectionDetection::detectNodesUpdateConnections(computerVision::ImageMat& imageInitial,
                                                       computerVision::ImageMat& imagePreprocessed,
                                                       const std::vector<circuit::Component>& components,
                                                       const bool saveImages)
{
    /*
     * Detection of nodes and update of connections
     * - For each component:
     *      - Increase 2 pixels to the dimensions of bounding box to allow intersection points with connections
     * - For each connection:
     *      - Get number of intersection points (N) between connection and bounding boxes of components
     *      - If N = 0, discard connection
     *      - If N <= 2, keep connection
     *      - If N > 2:
     *          - Create node (calculate its position)
     *          - Create N connections
     *          - Set connection wire
     *          - Set end ID for each connection with the node ID
     *          - Set the connections IDs of the node
     */

    mLogger->logInfo("Detecting nodes and update connections");

    const auto imgWidth{mOpenCvWrapper->getImageWidth(imagePreprocessed)};
    const auto imgHeight{mOpenCvWrapper->getImageHeight(imagePreprocessed)};
    constexpr int widthIncr{2};  // 2 pixels to allow centering
    constexpr int heightIncr{2}; // 2 pixels to allow centering

    // Copy of components
    auto componentsCopy{components};

    for (auto& component : componentsCopy) {
        // Increase dimensions of bounding box to allow intersection points with connections
        const auto box = increaseBoundingBox(component.mBoundingBox, widthIncr, heightIncr, imgWidth, imgHeight);
        component.mBoundingBox = box;
    }

    // Copy of connections
    const auto connectionsCopy{mConnections};
    // Clear connections and nodes because they will be updated
    mConnections.clear();
    mNodes.clear();

    for (const auto& connection : connectionsCopy) {
        std::vector<computerVision::Point> intersectionPoints{};

        for (const auto& component : componentsCopy) {
            auto intersect{false};

            for (const auto& point : connection.mWire) {
                // Intersection
                intersect = mOpenCvWrapper->contains(component.mBoundingBox, point);
                if (intersect) {
                    mLogger->logDebug("Intersection point between component and connection at {"
                                      + std::to_string(point.x) + ", " + std::to_string(point.y) + "}");

                    intersectionPoints.push_back(point);
                    // There is at least one intersection point, so no need to check other points
                    break;
                }
            }
        }

        const auto numIntersections{intersectionPoints.size()};
        mLogger->logDebug("Number of intersection points for this connection = " + std::to_string(numIntersections));

        if (numIntersections > 0 && numIntersections <= 2) {
            // Add connection
            mConnections.push_back(connection);
        }

        if (numIntersections > 2) {
            // Create node
            circuit::Node node{};

            // Set node position
            circuit::GlobalPosition pos{};
            const auto xPair{findExtremePoints(connection.mWire, true)};
            const auto yPair{findExtremePoints(connection.mWire, false)};
            // Center node position
            pos.mX = (xPair.first.x + xPair.second.x) / 2;
            pos.mY = (yPair.first.y + yPair.second.y) / 2;
            pos.mAngle = 0;
            node.mPosition = pos;

            mLogger->logDebug("Node position = {" + std::to_string(node.mPosition.mX) + ", "
                              + std::to_string(node.mPosition.mY) + "}");

            // Create new connections
            for (auto i{0}; i < numIntersections; ++i) {
                circuit::Connection newConnection{};

                // Set connection wire with 2 points: intersection and node
                circuit::Wire wire{intersectionPoints.at(i),
                                   computerVision::Point{node.mPosition.mX, node.mPosition.mY}};
                newConnection.mWire = wire;

                // Set connection end ID
                newConnection.mEndId = node.mId;
                // Set node connections ID
                node.mConnectionIds.push_back(newConnection.mId);

                // Add connections
                mConnections.push_back(newConnection);
            }

            // Set node type
            node.setType(circuit::Node::NodeType::REAL);

            // Add node
            mNodes.push_back(node);
        }
    }

    mLogger->logInfo("Connections detected in the circuit: " + std::to_string(mConnections.size()));
    mLogger->logInfo("Nodes detected in the circuit: " + std::to_string(mNodes.size()));

    // If there are no connections detected, it makes no sense to continue
    if (mConnections.empty()) {
        return false;
    }

    // Save image
    if (saveImages) {
        computerVision::ImageMat image = mOpenCvWrapper->cloneImage(imageInitial);

        // Connections
        computerVision::Contours connectionWires{};
        for (const auto& connection : mConnections) {
            connectionWires.push_back(connection.mWire);
        }
        mOpenCvWrapper->drawContours(image,
                                     connectionWires,
                                     -1,
                                     cConnectionColor,
                                     cConnectionThickness,
                                     computerVision::OpenCvWrapper::LineTypes::LINE_8,
                                     {});

        // Nodes
        if (!mNodes.empty()) {
            computerVision::Contours nodePoints{};
            for (const auto& node : mNodes) {
                computerVision::Point point{node.mPosition.mX, node.mPosition.mY};
                nodePoints.push_back(computerVision::Contour{point});
            }
            mOpenCvWrapper->drawContours(
                image, nodePoints, -1, cNodeColor, cNodeThickness, computerVision::OpenCvWrapper::LineTypes::LINE_8, {});
        }

        mOpenCvWrapper->writeImage("image_segment_detect_nodes.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Detecting nodes", image, 0);
    }

    return true;
}

const std::vector<circuit::Connection>& ConnectionDetection::getDetectedConnections() const
{
    return mConnections;
}

const std::vector<circuit::Node>& ConnectionDetection::getDetectedNodes() const
{
    return mNodes;
}

#ifdef BUILD_TESTS
void ConnectionDetection::setDetectedConnections(const std::vector<circuit::Connection>& connections)
{
    mConnections = connections;
}
#endif

} // namespace schematicSegmentation
} // namespace circuitSegmentation
