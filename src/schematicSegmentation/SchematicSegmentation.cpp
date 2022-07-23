/**
 * @file
 */

#include "SchematicSegmentation.h"
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

            mOpenCvWrapper->writeImage("image_segment_detect_ports.png", image);
            // TODO: Remove or comment.
            mOpenCvWrapper->showImage("Detecting components ports", image, 0);
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
