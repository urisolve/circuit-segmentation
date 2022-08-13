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
                                            const bool saveImages)
{
    /*
     * Detection of connections
     * - Generate an image with only the circuit connections (image A)
     *      - Morphological closing for dilation of circuit elements
     *      - Morphological opening to remove the circuit connections leaving only the dilated circuit elements (image
     * B)
     *      - Intersect the preprocessed image with the image B to obtain only the circuit elements (image C)
     *      - Find contours in image C
     *      - For each contour:
     *          - Generate a bounding box
     *          - Remove that box in the preprocessed image (bounding boxes with black pixels)
     * - Find contours in image A to identify each connection (wire = contour)
     * - For each contour:
     *      - Check contour length
     *      - If it has the minimum length, consider it as a connection
     */

    mLogger->logInfo("Detecting connections of the circuit");

    // Image used during the process
    computerVision::ImageMat image{};

    // Morphological closing for dilation of circuit elements
    auto kernelMorph{mOpenCvWrapper->getStructuringElement(computerVision::OpenCvWrapper::MorphShapes::MORPH_RECT,
                                                           cMorphCloseKernelSize)};
    mOpenCvWrapper->morphologyEx(
        imagePreprocessed, image, computerVision::OpenCvWrapper::MorphTypes::MORPH_CLOSE, kernelMorph, cMorphCloseIter);

    mLogger->logInfo("Morphological closing applied to the image");

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("cs_segment_connections_morph_close.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological closing to detect connections", image, 0);
    }

    // Morphological opening to remove the circuit connections leaving only the dilated circuit elements
    kernelMorph = mOpenCvWrapper->getStructuringElement(computerVision::OpenCvWrapper::MorphShapes::MORPH_RECT,
                                                        cMorphOpenKernelSize);
    mOpenCvWrapper->morphologyEx(
        image, image, computerVision::OpenCvWrapper::MorphTypes::MORPH_OPEN, kernelMorph, cMorphOpenIter);

    mLogger->logInfo("Morphological opening applied to the image");

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("cs_segment_connections_morph_open.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological opening to detect connections", image, 0);
    }

    // Intersect the preprocessed image with the image without circuit connections
    mOpenCvWrapper->bitwiseAnd(imagePreprocessed, image, image);

    mLogger->logInfo("Intersection between the preprocessed image and the image without connections");

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("cs_segment_connections_intersection.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Intersection between images to detect connections", image, 0);
    }

    // At this point, the circuit elements are in the image, so we need to find the contours
    computerVision::Contours contours{};
    computerVision::ContoursHierarchy hierarchy{};
    mOpenCvWrapper->findContours(image, contours, hierarchy, cFindContourMode, cFindContourMethod);

    mLogger->logDebug("Contours found in the intersection image: " + std::to_string(contours.size()));

    // Generate bounding box for each contour and remove it from the image
    image = mOpenCvWrapper->cloneImage(imagePreprocessed);
    for (const auto& contour : contours) {
        constexpr int widthIncr{2};  // 2 pixels to allow centering
        constexpr int heightIncr{2}; // 2 pixels to allow centering

        // Bounding box
        const auto box{generateBoundingBox(mOpenCvWrapper, contour, imagePreprocessed, widthIncr, heightIncr)};

        // Remove box (bounding box with black pixels)
        mOpenCvWrapper->rectangle(image, box, {0, 0, 0}, -1, computerVision::OpenCvWrapper::LineTypes::LINE_8);
    }

    mLogger->logInfo("Generated image with only the circuit connections");

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("cs_segment_connections_only_conn.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Image with only the circuit connections to detect connections", image, 0);
    }

    // At this point, the connections are represented as wires in the image, so we need to find those wires
    computerVision::Contours wires{};
    mOpenCvWrapper->findContours(image, wires, hierarchy, cFindContourMode, cFindContourMethod);

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

        mOpenCvWrapper->writeImage("cs_segment_connections_detected.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Detecting connections", image, 0);
    }

    return true;
}

bool ConnectionDetection::updateConnections(computerVision::ImageMat& imageInitial,
                                            computerVision::ImageMat& imagePreprocessed,
                                            const std::vector<circuit::Component>& components,
                                            const bool saveImages)
{
    /*
     * Update of detected connections
     * - Remove the detected components (set components with black pixels)
     * - Find contours to identify each connection (wire = contour)
     * - For each contour:
     *      - Check contour length
     *      - If it has the minimum length, consider it as a connection
     */

    mLogger->logInfo("Updating connections of the circuit");

    // Image used during the process
    computerVision::ImageMat image{};

    // Remove the detected components (set components with black pixels)
    image = mOpenCvWrapper->cloneImage(imagePreprocessed);
    for (const auto& component : components) {
        mOpenCvWrapper->rectangle(
            image, component.mBoundingBox, {0, 0, 0}, -1, computerVision::OpenCvWrapper::LineTypes::LINE_8);
    }

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("image_segment_connections_remove_components.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Remove components", image, 0);
    }

    // At this point, the connections are represented as wires in the image, so we need to find those wires
    computerVision::Contours wires{};
    computerVision::ContoursHierarchy hierarchy{};
    mOpenCvWrapper->findContours(image, wires, hierarchy, cFindContourMode, cFindContourMethod);

    mLogger->logDebug("Contours found in the image, to update connections: " + std::to_string(wires.size()));

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

        mOpenCvWrapper->writeImage("image_segment_connections_updated.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Updating connections", image, 0);
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
                    // There is at least one intersection point, so no need to check other points of this wire
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

        mOpenCvWrapper->writeImage("cs_segment_nodes_detected_connections_updated.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Detecting nodes and updating connections", image, 0);
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
