/**
 * @file
 */

#include "ComponentDetection.h"
#include "SegmentationUtils.h"

namespace circuitSegmentation {
namespace schematicSegmentation {

ComponentDetection::ComponentDetection(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                       const std::shared_ptr<logging::Logger>& logger)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
    , mComponents{}
{
}

bool ComponentDetection::detectComponents(computerVision::ImageMat& imageInitial,
                                          computerVision::ImageMat& imagePreprocessed,
                                          const std::vector<circuit::Connection>& connections,
                                          const bool saveImages)
{
    /*
     * Detection of components
     * - Remove the connections from the preprocessed image (set connections with black pixels)
     * - Morphological closing for dilation of circuit elements (this is particularly useful for components with
     * disconnected lines, like ground, capacitor, etc)
     * - Find contours in the image after dilation of circuit elements
     * - For each contour:
     *      - Generate a bounding box
     *      - Increase 2 pixels to the dimensions of bounding box to allow intersection points with connections
     *      - For each bounding box:
     *          - Check bounding box area
     *          - If the bounding box has the minimum area, check if the box has intersection points with connections
     *              - If yes, save the bounding box and consider as a component
     */

    mLogger->logInfo("Detecting components");

    // Image used during the process
    computerVision::ImageMat image{mOpenCvWrapper->cloneImage(imagePreprocessed)};

    // Remove the connections from the preprocessed image (set connections with black pixels)
    removeConnectionsFromImage(image, connections);

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("cs_segment_components_remove_connections.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Removing the connections from the preprocessed image", image, 0);
    }

    // Morphological closing for dilation of circuit elements
    auto kernelMorph{mOpenCvWrapper->getStructuringElement(computerVision::OpenCvWrapper::MorphShapes::MORPH_RECT,
                                                           cMorphCloseKernelSize)};
    mOpenCvWrapper->morphologyEx(
        image, image, computerVision::OpenCvWrapper::MorphTypes::MORPH_CLOSE, kernelMorph, cMorphCloseIter);

    mLogger->logInfo("Morphological closing applied to the image");

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("cs_segment_components_morph_close.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological closing to detect components", image, 0);
    }

    // At this point, the circuit elements are in the image, so we need to find the contours
    computerVision::Contours contours{};
    computerVision::ContoursHierarchy hierarchy{};
    mOpenCvWrapper->findContours(image, contours, hierarchy, cFindContourMode, cFindContourMethod);

    mLogger->logDebug("Contours found in the image, to detect components: " + std::to_string(contours.size()));

    for (const auto& contour : contours) {
        // Check contour
        const auto box{checkContour(imagePreprocessed, contour, connections)};

        if (box.has_value()) {
            // Add component
            circuit::Component component{};
            component.mBoundingBox = box.value();
            mComponents.push_back(component);
        }
    }

    mLogger->logInfo("Components found in the circuit: " + std::to_string(mComponents.size()));

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
                                      cBoxColor,
                                      cBoxThickness,
                                      computerVision::OpenCvWrapper::LineTypes::LINE_8);
        }

        mOpenCvWrapper->writeImage("cs_segment_components_detected.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Detecting components", image, 0);
    }

    return true;
}

const std::vector<circuit::Component>& ComponentDetection::getDetectedComponents() const
{
    return mComponents;
}

void ComponentDetection::removeConnectionsFromImage(computerVision::ImageMat& image,
                                                    const std::vector<circuit::Connection>& connections)
{
    computerVision::Contours wires{};
    for (const auto& connection : connections) {
        wires.push_back(connection.mWire);
    }
    if (!wires.empty()) {
        mOpenCvWrapper->drawContours(
            image, wires, -1, {0, 0, 0}, -1, computerVision::OpenCvWrapper::LineTypes::LINE_8, {});
    }
}

std::optional<computerVision::Rectangle>
    ComponentDetection::checkContour(computerVision::ImageMat& imagePreprocessed,
                                     const computerVision::Contour& contour,
                                     const std::vector<circuit::Connection>& connections)
{
    constexpr int widthIncr{2};  // 2 pixels to allow centering
    constexpr int heightIncr{2}; // 2 pixels to allow centering

    // Bounding box
    const auto box{generateBoundingBox(mOpenCvWrapper, contour, imagePreprocessed, widthIncr, heightIncr)};

    auto intersect{false};

    // Check bounding box area
    if (mOpenCvWrapper->rectangleArea(box) >= cBoxMinArea) {
        // Check if the box has intersection points with connections
        for (const auto& connection : connections) {
            for (const auto& point : connection.mWire) {
                // Intersection
                intersect = mOpenCvWrapper->contains(box, point);
                if (intersect) {
                    mLogger->logDebug("Intersection point between contour and connection at {" + std::to_string(point.x)
                                      + ", " + std::to_string(point.y) + "}");

                    // There is at least one intersection point, so no need to check other points of this wire
                    break;
                }
            }

            if (intersect) {
                // There is at least one intersection point, so no need to check other connections
                break;
            }
        }
    }

    if (intersect) {
        return box;
    }

    return std::nullopt;
}

} // namespace schematicSegmentation
} // namespace circuitSegmentation
