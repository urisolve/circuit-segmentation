/**
 * @file
 */

#include "ComponentSegmentation.h"
#include <algorithm>

namespace circuitSegmentation {
namespace schematicSegmentation {

using namespace circuitSegmentation::computerVision;

ComponentSegmentation::ComponentSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                             const std::shared_ptr<logging::Logger>& logger)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
    , mComponents{}
{
}

bool ComponentSegmentation::detectComponents(computerVision::ImageMat& imageInitial,
                                             computerVision::ImageMat& imagePreprocessed,
                                             const bool saveImages)
{
    /*
     * Detection of components
     * - Morphological closing is applied which results in dilating components
     * - Morphological opening removes the rest of the circuit leaving only the dilated components
     * - Save bounding boxes of the components identified
     */

    mLogger->logInfo("Detecting components");

    // Image used during the process
    computerVision::ImageMat image{};

    // Morphological closing for dilation of components
    auto kernelMorph{
        mOpenCvWrapper->getStructuringElement(OpenCvWrapper::MorphShapes::MORPH_RECT, cMorphCloseKernelSize)};
    mOpenCvWrapper->morphologyEx(
        imagePreprocessed, image, OpenCvWrapper::MorphTypes::MORPH_CLOSE, kernelMorph, cMorphCloseIter);

    mLogger->logInfo("Morphological closing applied to the image, to detect components");

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("image_segment_morph_close.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological closing to detect components", image, 0);
    }

    // Morphological opening to remove the rest of the circuit leaving only the dilated components
    kernelMorph = mOpenCvWrapper->getStructuringElement(OpenCvWrapper::MorphShapes::MORPH_RECT, cMorphOpenKernelSize);
    mOpenCvWrapper->morphologyEx(image, image, OpenCvWrapper::MorphTypes::MORPH_OPEN, kernelMorph, cMorphOpenIter);

    mLogger->logInfo("Morphological opening applied to the image, to detect components");

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("image_segment_morph_open.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological opening to detect components", image, 0);
    }

    // At this point, the components are represented as blobs in the image, so we need to find those blobs

    Contours componentsBlobs{};
    ContoursHierarchy hierarchy{};

    mOpenCvWrapper->findContours(
        image, componentsBlobs, hierarchy, cBoundingBoxFindContourMode, cBoundingBoxFindContourMethod);

    mLogger->logDebug("Contours found in the image, to detect components: " + std::to_string(componentsBlobs.size()));

    // Generate bounding boxes for blobs
    boundingBoxComponents(componentsBlobs, imagePreprocessed);

    mLogger->logInfo("Components found in the image: " + std::to_string(mComponents.size()));

    // If there are no components detected, it makes no sense to continue
    if (mComponents.empty()) {
        return false;
    }

    // Save image
    if (saveImages) {
        image = mOpenCvWrapper->cloneImage(imageInitial);
        for (const auto& component : mComponents) {
            mOpenCvWrapper->rectangle(image,
                                      component.mBoundingBox,
                                      cBoundingBoxColor,
                                      cBoundingBoxThickness,
                                      OpenCvWrapper::LineTypes::LINE_8);
        }

        mOpenCvWrapper->writeImage("image_segment_detect_components.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Detecting components", image, 0);
    }

    return true;
}

void ComponentSegmentation::detectComponentConnections(computerVision::ImageMat& imagePreprocessed,
                                                       const std::vector<circuit::Connection>& connections)
{
    /*
     * Detection of component connections
     * - For each component:
     *      - Increase 2 pixels to the dimensions of bounding box to allow intersection points with connections
     *      - For each connection:
     *          - Check if component bounding box contains connection points
     *          - If it contains, there is connection between the component and the connection (add connection to that
     * component)
     */

    mLogger->logInfo("Detecting component connection points");

    const auto imgWidth{mOpenCvWrapper->getImageWidth(imagePreprocessed)};
    const auto imgHeight{mOpenCvWrapper->getImageHeight(imagePreprocessed)};
    constexpr int widthIncr{2};  // 2 pixels to allow centering
    constexpr int heightIncr{2}; // 2 pixels to allow centering

    for (auto& component : mComponents) {
        // Increase dimensions of bounding box to allow intersection points with connections
        const auto box = increaseBoundingBox(component.mBoundingBox, widthIncr, heightIncr, imgWidth, imgHeight);

        std::vector<circuit::Connection> connectionsConnected{};

        mLogger->logDebug("Checking component with top-left corner (" + std::to_string(component.mBoundingBox.x) + ", "
                          + std::to_string(component.mBoundingBox.y) + ")");

        for (const auto& connection : connections) {
            const auto wire{connection.mWire};
            auto intersect{false};

            for (const auto& point : wire) {
                // Intersection
                intersect = mOpenCvWrapper->contains(box, point);
                if (intersect) {
                    mLogger->logDebug("Component connected to a connection wire at point (" + std::to_string(point.x)
                                      + ", " + std::to_string(point.y) + ")");

                    // There is at least one intersection point, so no need to check other points
                    break;
                }
            }

            if (intersect) {
                // Add connection
                connectionsConnected.push_back(connection);
            }
        }

        // Set connections to the component
        component.mConnections = connectionsConnected;
    }
}

bool ComponentSegmentation::updateDetectedComponents()
{
    /*
     * Update list of detected components
     * - Discard components that are not connected to circuit connections
     */

    mLogger->logInfo("Updating list of detected components");

    // Verify that the component is not connected to a connection
    const auto isComponentNotConnected = [](circuit::Component component) {
        const auto connections{component.mConnections};
        return connections.empty();
    };

    // Erase components from the vector that are not connected to a connection
    mComponents.erase(std::remove_if(mComponents.begin(), mComponents.end(), isComponentNotConnected),
                      mComponents.end());

    mLogger->logInfo("Detected components in the image: " + std::to_string(mComponents.size()));

    // Return if there are still components in the vector
    return !mComponents.empty();
}

const std::vector<circuit::Component>& ComponentSegmentation::getDetectedComponents() const
{
    return mComponents;
}

void ComponentSegmentation::boundingBoxComponents(const computerVision::Contours& blobs,
                                                  computerVision::ImageMat& imagePreprocessed)
{
    const auto imgWidth{mOpenCvWrapper->getImageWidth(imagePreprocessed)};
    const auto imgHeight{mOpenCvWrapper->getImageHeight(imagePreprocessed)};

    mComponents.clear();

    for (const auto& blob : blobs) {
        // Check blob area
        if (mOpenCvWrapper->contourArea(blob) < cBoundingBoxMinArea) {
            continue;
        }

        // Bounding rectangle
        auto rect{mOpenCvWrapper->boundingRect(blob)};

        // Increase dimensions because bounding boxes may not completely enclose the component
        rect = increaseBoundingBox(rect, cBoundingBoxWidthIncrease, cBoundingBoxHeightIncrease, imgWidth, imgHeight);

        // Add component
        circuit::Component component{};
        component.mBoundingBox = rect;
        mComponents.push_back(component);
    }
}

computerVision::Rectangle ComponentSegmentation::increaseBoundingBox(const computerVision::Rectangle& box,
                                                                     const int& widthIncr,
                                                                     const int& heightIncr,
                                                                     const int& widthMax,
                                                                     const int& heightMax)
{
    // Axis
    auto x{box.x - widthIncr / 2};  // Truncated
    x = x < 0 ? 0 : x;              // Cannot be negative
    auto y{box.y - heightIncr / 2}; // Truncated
    y = y < 0 ? 0 : y;              // Cannot be negative

    // Dimensions
    auto width{box.width + widthIncr};
    if ((x + width) > widthMax) {
        width = widthMax - x;
    }
    auto height{box.height + heightIncr};
    if ((y + height) > heightMax) {
        height = heightMax - y;
    }

    return Rectangle{x, y, width, height};
}

#ifdef BUILD_TESTS
void ComponentSegmentation::setDetectedComponents(const std::vector<circuit::Component>& components)
{
    mComponents = components;
}
#endif

} // namespace schematicSegmentation
} // namespace circuitSegmentation
