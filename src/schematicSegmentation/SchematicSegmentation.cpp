/**
 * @file
 */

#include "SchematicSegmentation.h"
#include "application/Config.h"
#include "SegmentationUtils.h"

namespace circuitSegmentation {
namespace schematicSegmentation {

SchematicSegmentation::SchematicSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                             const std::shared_ptr<logging::Logger>& logger)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
    , mComponents{}
    , mConnections{}
    , mNodes{}
    , mLabels{}
{
}

void SchematicSegmentation::detectComponentConnections(computerVision::ImageMat& imageInitial,
                                                       computerVision::ImageMat& imagePreprocessed,
                                                       const std::vector<circuit::Component>& componentsDetected,
                                                       const std::vector<circuit::Connection>& connectionsDetected,
                                                       const std::vector<circuit::Node>& nodesDetected,
                                                       const bool saveImages)
{
    /*
     * Detection of component connections
     * - For each component:
     *      - Increase 2 pixels to the dimensions of bounding box to allow intersection points with connections
     *      - For each connection:
     *          - Check if component bounding box contains connection points
     *          - If it contains, there is connection with the component
     *              - Add component port (owner ID, connection ID and position)
     *              - Check connection start ID:
     *                  - If empty, set start ID with port ID
     *                  - If not empty, set end ID with port ID
     */

    mLogger->logInfo("Detecting connection points (ports) of components");

    mComponents = componentsDetected;
    mConnections = connectionsDetected;
    mNodes = nodesDetected;

    const auto imgWidth{mOpenCvWrapper->getImageWidth(imagePreprocessed)};
    const auto imgHeight{mOpenCvWrapper->getImageHeight(imagePreprocessed)};
    constexpr int widthIncr{2};  // 2 pixels to allow centering
    constexpr int heightIncr{2}; // 2 pixels to allow centering

    for (auto& component : mComponents) {
        // Increase dimensions of bounding box to allow intersection points with connections
        const auto box = increaseBoundingBox(component.mBoundingBox, widthIncr, heightIncr, imgWidth, imgHeight);

        std::vector<circuit::Connection> componentConnections{};

        mLogger->logDebug("Checking component with ID " + component.mId);

        for (auto& connection : mConnections) {
            auto intersect{false};
            computerVision::Point intersectionPoint{};

            for (const auto& point : connection.mWire) {
                // Intersection
                intersect = mOpenCvWrapper->contains(box, point);
                if (intersect) {
                    mLogger->logDebug("Component connected to a connection wire at point {" + std::to_string(point.x)
                                      + ", " + std::to_string(point.y) + "}");

                    intersectionPoint.x = point.x;
                    intersectionPoint.y = point.y;
                    // There is at least one intersection point, so no need to check other points
                    break;
                }
            }

            if (intersect) {
                // Create port
                circuit::Port port{};

                // Set port owner ID
                port.mOwnerId = component.mId;

                // Set port connection ID
                port.mConnectionId = connection.mId;

                // Set port position
                port.mPosition = calcPortPosition(intersectionPoint, component.mBoundingBox, widthIncr, heightIncr);
                mLogger->logDebug("Port position at {" + std::to_string(port.mPosition.mX) + ", "
                                  + std::to_string(port.mPosition.mY) + "}");

                // Add component port
                component.mPorts.push_back(port);

                // Set connection start/end ID
                if (connection.mStartId.empty()) {
                    connection.mStartId = port.mId;
                } else {
                    connection.mEndId = port.mId;
                }
            }
        }
    }

    // Save image
    if (saveImages) {
        computerVision::Contours portPoints{};
        for (const auto& component : mComponents) {
            for (const auto& port : component.mPorts) {
                computerVision::Point point{
                    static_cast<int>(component.mBoundingBox.x + port.mPosition.mX * component.mBoundingBox.width),
                    static_cast<int>(component.mBoundingBox.y + port.mPosition.mY * component.mBoundingBox.height)};
                portPoints.push_back(computerVision::Contour{point});
            }
        }

        if (!portPoints.empty()) {
            computerVision::ImageMat image = mOpenCvWrapper->cloneImage(imageInitial);

            mOpenCvWrapper->drawContours(
                image, portPoints, -1, cPortColor, cPortThickness, computerVision::OpenCvWrapper::LineTypes::LINE_8, {});

            mOpenCvWrapper->writeImage("cs_segment_components_ports_detected.png", image);
#ifdef SHOW_IMAGES
            mOpenCvWrapper->showImage("Detecting components ports", image, 0);
#endif
        }
    }
}

bool SchematicSegmentation::updateDetectedComponents()
{
    /*
     * Update list of detected components
     * - Discard components which do not have ports
     * - Set components positions
     */

    mLogger->logInfo("Updating list of detected components");

    // Verify that the component does not have a port
    const auto isComponentWithoutPorts = [](circuit::Component component) {
        const auto ports{component.mPorts};
        return ports.empty();
    };

    // Erase components from the vector that do not have a port
    mComponents.erase(std::remove_if(mComponents.begin(), mComponents.end(), isComponentWithoutPorts),
                      mComponents.end());

    mLogger->logInfo("Detected components in the image: " + std::to_string(mComponents.size()));

    for (auto& component : mComponents) {
        // Set component position
        component.mPosition.mX = component.mBoundingBox.x;
        component.mPosition.mY = component.mBoundingBox.y;
        component.mPosition.mAngle = 0;
    }

    // Return if there are still components in the vector
    return !mComponents.empty();
}

void SchematicSegmentation::associateLabels(computerVision::ImageMat& imageInitial,
                                            computerVision::ImageMat& imagePreprocessed,
                                            const std::vector<circuit::Label>& labelsDetected,
                                            const bool saveImages)
{
    /*
     * Association of labels to the elements of the circuit
     * - Create bounding boxes for connections
     * - Create bounding boxes for nodes
     * - For each label:
     *      - Get and check the minimum distance between label and component
     *      - Get and check the minimum distance between label and connection
     *      - Get and check the minimum distance between label and node
     *      - Compare minimum distances of each element
     *      - Set label position
     *      - Set label owner ID with the element ID with the minimum distance
     *      - Add to the labels vector of that element
     *      - Set label of that element (label of the element is equal to the last associated label)
     */

    mLogger->logInfo("Associating labels to the circuit elements");

    mLabels = labelsDetected;

    // Create bounding boxes for connections
    std::vector<computerVision::Rectangle> connectionsBoxes{};
    for (const auto& connection : mConnections) {
        constexpr int widthIncr{2};  // 2 pixels to allow centering
        constexpr int heightIncr{2}; // 2 pixels to allow centering
        const auto box{generateBoundingBox(mOpenCvWrapper, connection.mWire, imagePreprocessed, widthIncr, heightIncr)};
        connectionsBoxes.push_back(box);
    }

    // Create bounding boxes for nodes
    std::vector<computerVision::Rectangle> nodesBoxes{};
    for (const auto& node : mNodes) {
        // Node point
        const computerVision::Point point{node.mPosition.mX, node.mPosition.mY};

        constexpr int widthIncr{20};  // 20 pixels to allow centering and to expand node area
        constexpr int heightIncr{20}; // 20 pixels to allow centering and to expand node area
        const auto box{generateBoundingBox(
            mOpenCvWrapper, computerVision::Contour{point}, imagePreprocessed, widthIncr, heightIncr)};
        nodesBoxes.push_back(box);
    }

    // Save image
    if (saveImages) {
        if (!connectionsBoxes.empty() || !nodesBoxes.empty()) {
            computerVision::ImageMat image{mOpenCvWrapper->cloneImage(imageInitial)};
            for (const auto& box : connectionsBoxes) {
                mOpenCvWrapper->rectangle(
                    image, box, cBoxColor, cBoxThickness, computerVision::OpenCvWrapper::LineTypes::LINE_8);
            }
            for (const auto& box : nodesBoxes) {
                mOpenCvWrapper->rectangle(
                    image, box, cBoxColor, cBoxThickness, computerVision::OpenCvWrapper::LineTypes::LINE_8);
            }

            mOpenCvWrapper->writeImage("cs_segment_labels_associate_boxes_connections_nodes.png", image);
#ifdef SHOW_IMAGES
            mOpenCvWrapper->showImage("Associating labels (boxes for connections and nodes)", image, 0);
#endif
        }
    }

    for (auto& label : mLabels) {
        // Distance between label and components
        double minDistanceToComponent{0};
        auto componentIndex{0};
        for (auto it{mComponents.begin()}; it != mComponents.end(); ++it) {
            const auto distance = schematicSegmentation::distanceRectangles(label.mBoundingBox, it->mBoundingBox);

            const auto index{it - mComponents.begin()};
            if (index == 0) {
                // For the first iteration, the distance calculated is the minimum distance
                minDistanceToComponent = distance;
                componentIndex = index;
            } else if (distance < minDistanceToComponent) {
                minDistanceToComponent = distance;
                componentIndex = index;
            }
        }

        mLogger->logDebug("Minimum distance between label " + label.mId + " and component "
                          + mComponents.at(componentIndex).mId + " = " + std::to_string(minDistanceToComponent));

        // Distance between label and connections
        double minDistanceToConnection{0};
        auto connectionIndex{0};
        for (auto it{connectionsBoxes.begin()}; it != connectionsBoxes.end(); ++it) {
            const auto index{it - connectionsBoxes.begin()};
            const auto distance
                = schematicSegmentation::distanceRectangles(label.mBoundingBox, connectionsBoxes.at(index));

            if (index == 0) {
                // For the first iteration, the distance calculated is the minimum distance
                minDistanceToConnection = distance;
                connectionIndex = index;
            } else if (distance < minDistanceToConnection) {
                minDistanceToConnection = distance;
                connectionIndex = index;
            }
        }

        mLogger->logDebug("Minimum distance between label " + label.mId + " and connection "
                          + mConnections.at(connectionIndex).mId + " = " + std::to_string(minDistanceToConnection));

        // Distance between label and nodes
        double minDistanceToNode{0};
        auto nodeIndex{0};
        for (auto it{nodesBoxes.begin()}; it != nodesBoxes.end(); ++it) {
            const auto index{it - nodesBoxes.begin()};
            const auto distance = schematicSegmentation::distanceRectangles(label.mBoundingBox, nodesBoxes.at(index));

            if (index == 0) {
                // For the first iteration, the distance calculated is the minimum distance
                minDistanceToNode = distance;
                nodeIndex = index;
            } else if (distance < minDistanceToNode) {
                minDistanceToNode = distance;
                nodeIndex = index;
            }
        }

        // The circuit can have no nodes, so we need to check if nodes are empty
        if (!mNodes.empty()) {
            mLogger->logDebug("Minimum distance between label " + label.mId + " and node " + mNodes.at(nodeIndex).mId
                              + " = " + std::to_string(minDistanceToNode));
        }

        // Compare the minimum distances
        auto distance{minDistanceToComponent};
        auto elemIndex{componentIndex};
        enum class ElemTypeEnum : unsigned char { COMPONENT, CONNECTION, NODE };
        auto elemType{ElemTypeEnum::COMPONENT};
        if (minDistanceToConnection < distance) {
            distance = minDistanceToConnection;
            elemIndex = connectionIndex;
            elemType = ElemTypeEnum::CONNECTION;
        }
        if (minDistanceToNode < distance && !mNodes.empty()) {
            distance = minDistanceToNode;
            elemIndex = nodeIndex;
            elemType = ElemTypeEnum::NODE;
        }

        // Set label position
        label.mPosition.mX = label.mBoundingBox.x;
        label.mPosition.mY = label.mBoundingBox.y;
        label.mPosition.mAngle = 0;

        switch (elemType) {
        case ElemTypeEnum::COMPONENT:
            // Set label owner ID
            label.mOwnerId = mComponents.at(elemIndex).mId;
            // Add to the labels vector
            mComponents.at(elemIndex).mLabels.push_back(label);
            // Set label of the element
            mComponents.at(elemIndex).mLabel = label;
            mLogger->logDebug("Label " + label.mId + " is associated to the component "
                              + mComponents.at(elemIndex).mId);
            break;
        case ElemTypeEnum::CONNECTION:
            // Set label owner ID
            label.mOwnerId = mConnections.at(elemIndex).mId;
            // Add to the labels vector
            mConnections.at(elemIndex).mLabels.push_back(label);
            // Set label of the element
            mConnections.at(elemIndex).mLabel = label;
            mLogger->logDebug("Label " + label.mId + " is associated to the connection "
                              + mConnections.at(elemIndex).mId);
            break;
        case ElemTypeEnum::NODE:
            // Set label owner ID
            label.mOwnerId = mNodes.at(elemIndex).mId;
            // Add to the labels vector
            mNodes.at(elemIndex).mLabels.push_back(label);
            // Set label of the element
            mNodes.at(elemIndex).mLabel = label;
            mLogger->logDebug("Label " + label.mId + " is associated to the node " + mNodes.at(elemIndex).mId);
            break;
        default:
            // Set label owner ID
            label.mOwnerId = mComponents.at(elemIndex).mId;
            // Add to the labels vector
            mComponents.at(elemIndex).mLabels.push_back(label);
            // Set label of the element
            mComponents.at(elemIndex).mLabel = label;
            mLogger->logDebug("Label " + label.mId + " is associated to the component "
                              + mComponents.at(elemIndex).mId);
            break;
        }
    }
}

const std::vector<circuit::Component>& SchematicSegmentation::getComponents() const
{
    return mComponents;
}

const std::vector<circuit::Connection>& SchematicSegmentation::getConnections() const
{
    return mConnections;
}

const std::vector<circuit::Node>& SchematicSegmentation::getNodes() const
{
    return mNodes;
}

const std::vector<circuit::Label>& SchematicSegmentation::getLabels() const
{
    return mLabels;
}

circuit::RelativePosition SchematicSegmentation::calcPortPosition(const computerVision::Point& connectionPoint,
                                                                  const computerVision::Rectangle& box,
                                                                  const int widthIncr,
                                                                  const int heightIncr)
{
    circuit::RelativePosition pos{};

    // Lambda for calculation of relative x
    const auto calcRelX = [&connectionPoint, &box]() {
        return 1 - (static_cast<double>(box.x + box.width - connectionPoint.x) / box.width);
    };

    // X axis
    if (connectionPoint.x >= (box.x - widthIncr) && connectionPoint.x <= (box.x)) {
        // Port is on the left
        pos.mX = 0;
    } else if (connectionPoint.x >= (box.x + box.width) && connectionPoint.x <= (box.x + box.width + widthIncr)) {
        // Port is on the right
        pos.mX = 1;
    } else {
        // Port is on the middle
        pos.mX = calcRelX();
    }

    // Lambda for calculation of relative y
    const auto calcRelY = [&connectionPoint, &box]() {
        return 1 - (static_cast<double>(box.y + box.height - connectionPoint.y) / box.height);
    };

    // Y axis
    if (connectionPoint.y >= (box.y - heightIncr) && connectionPoint.y <= (box.y)) {
        // Port is on the top
        pos.mY = 0;
    } else if (connectionPoint.y >= (box.y + box.height) && connectionPoint.y <= (box.y + box.height + heightIncr)) {
        // Port is on the bottom
        pos.mY = 1;
    } else {
        // Port is on the middle
        pos.mY = calcRelY();
    }

    return pos;
}

} // namespace schematicSegmentation
} // namespace circuitSegmentation
