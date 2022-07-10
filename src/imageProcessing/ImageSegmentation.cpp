/**
 * @file
 */

#include "ImageSegmentation.h"
#include <algorithm>

namespace circuitSegmentation {
namespace imageProcessing {

using namespace circuitSegmentation::computerVision;

ImageSegmentation::ImageSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                     const std::shared_ptr<logging::Logger>& logger,
                                     const bool saveImages)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
    , mSaveImages{std::move(saveImages)}
    , mComponents{}
    , mNodes{}
{
}

bool ImageSegmentation::segmentImage(computerVision::ImageMat imageInitial, computerVision::ImageMat imagePreprocessed)
{
    mLogger->logInfo("Starting image segmentation");

    mImageInitial = std::move(imageInitial);
    mImagePreprocessed = std::move(imagePreprocessed);

    // TODO: Update segmentation process to fill closed shapes.
    /*
     * Steps:
     * - Detection of components
     *     - Morphological closing is applied which results in dilating components
     *     - Morphological opening removes the rest of the circuit leaving only the dilated components
     *     - Save bounding boxes of the components identified
     * - Detection of nodes
     *     - Separation of components found and circuit (bounding boxes as black pixels), so the components are removed
     *     - Contour finding algorithm is applied to identify each node (wire = contour)
     * - Circuit connections
     *     - For each component:
     *         - Increase 2 pixels to the dimensions of bounding box to allow intersection points with nodes
     *         - For each node:
     *             - Check if component bounding box contains node points
     *             - If it contains, there is connection between the component and the node (add node to that component)
     * - Update list of detected components
     *     - Discard components that are not connected to circuit nodes
     */

    // Detect components
    if (!detectComponents()) {
        return false;
    }

    // Detect nodes
    if (!detectNodes()) {
        return false;
    }

    // Detect circuit connections
    detectCircuitConnections();

    // Update list of detected components
    if (!updateDetectedComponents()) {
        return false;
    }

    return true;
}

void ImageSegmentation::setSaveImages(const bool& saveImages)
{
    mSaveImages = saveImages;
}

bool ImageSegmentation::getSaveImages() const
{
    return mSaveImages;
}

bool ImageSegmentation::detectComponents()
{
    mLogger->logInfo("Detecting components");

    // Morphological closing for dilation of components
    auto kernelMorph{
        mOpenCvWrapper->getStructuringElement(OpenCvWrapper::MorphShapes::MORPH_RECT, cMorphCloseKernelSize)};
    mOpenCvWrapper->morphologyEx(mImagePreprocessed,
                                 mImageSegmentationProc,
                                 OpenCvWrapper::MorphTypes::MORPH_CLOSE,
                                 kernelMorph,
                                 cMorphCloseIter);

    mLogger->logInfo("Morphological closing applied to the image, to detect components");

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_segment_morph_close.png", mImageSegmentationProc);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological closing to detect components", mImageSegmentationProc, 0);
    }

    // Morphological opening to remove the rest of the circuit leaving only the dilated components
    kernelMorph = mOpenCvWrapper->getStructuringElement(OpenCvWrapper::MorphShapes::MORPH_RECT, cMorphOpenKernelSize);
    mOpenCvWrapper->morphologyEx(mImageSegmentationProc,
                                 mImageSegmentationProc,
                                 OpenCvWrapper::MorphTypes::MORPH_OPEN,
                                 kernelMorph,
                                 cMorphOpenIter);

    mLogger->logInfo("Morphological opening applied to the image, to detect components");

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_segment_morph_open.png", mImageSegmentationProc);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological opening to detect components", mImageSegmentationProc, 0);
    }

    // At this point, the components are represented as blobs in the image, so we need to find those blobs

    Contours componentsBlobs{};
    ContoursHierarchy hierarchy{};

    mOpenCvWrapper->findContours(
        mImageSegmentationProc, componentsBlobs, hierarchy, cBoundingBoxFindContourMode, cBoundingBoxFindContourMethod);

    mLogger->logDebug("Contours found in the image, to detect components: " + std::to_string(componentsBlobs.size()));

    // Generate bounding boxes for blobs
    boundingBoxComponents(componentsBlobs);

    mLogger->logInfo("Components found in the image: " + std::to_string(mComponents.size()));

    // If there are no components detected, it makes no sense to continue
    if (mComponents.empty()) {
        return false;
    }

    // Save image
    if (mSaveImages) {
        mImageSegmentationProc = mOpenCvWrapper->cloneImage(mImageInitial);
        for (const auto& component : mComponents) {
            mOpenCvWrapper->rectangle(mImageSegmentationProc,
                                      component.getBoundingBox(),
                                      cBoundingBoxColor,
                                      cBoundingBoxThickness,
                                      OpenCvWrapper::LineTypes::LINE_8);
        }

        mOpenCvWrapper->writeImage("image_segment_detect_components.png", mImageSegmentationProc);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Detecting components", mImageSegmentationProc, 0);
    }

    return true;
}

void ImageSegmentation::boundingBoxComponents(const computerVision::Contours& blobs)
{
    const auto imgWidth{mOpenCvWrapper->getImageWidth(mImagePreprocessed)};
    const auto imgHeight{mOpenCvWrapper->getImageHeight(mImagePreprocessed)};

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
        component.setBoundingBox(rect);
        mComponents.push_back(component);
    }
}

computerVision::Rectangle ImageSegmentation::increaseBoundingBox(const computerVision::Rectangle& box,
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

bool ImageSegmentation::detectNodes()
{
    mLogger->logInfo("Detecting nodes of the circuit");

    // Set bounding boxes as black pixels, so the components are removed
    mImageSegmentationProc = mOpenCvWrapper->cloneImage(mImagePreprocessed);
    for (const auto& component : mComponents) {
        mOpenCvWrapper->rectangle(
            mImageSegmentationProc, component.getBoundingBox(), {0, 0, 0}, -1, OpenCvWrapper::LineTypes::LINE_8);
    }

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_segment_remove_components.png", mImageSegmentationProc);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Remove components", mImageSegmentationProc, 0);
    }

    // At this point, the nodes are represented as wires in the image, so we need to find those wires

    Contours wires{};
    ContoursHierarchy hierarchy{};

    mOpenCvWrapper->findContours(
        mImageSegmentationProc, wires, hierarchy, cNodesFindContourMode, cNodesFindContourMethod);

    mLogger->logDebug("Contours found in the image, to detect nodes: " + std::to_string(wires.size()));

    // Wire for each node

    mNodes.clear();

    for (const auto& wire : wires) {
        // Check wire length
        if (mOpenCvWrapper->arcLength(wire, false) >= cNodesMinLength) {
            // Add node
            circuit::Node node{};
            node.setWire(wire);
            mNodes.push_back(node);
        }
    }

    mLogger->logInfo("Nodes found in the circuit: " + std::to_string(mNodes.size()));

    // If there are no nodes detected, it makes no sense to continue
    if (mNodes.empty()) {
        return false;
    }

    // Save image
    if (mSaveImages) {
        mImageSegmentationProc = mOpenCvWrapper->cloneImage(mImageInitial);
        Contours wires{};
        for (const auto& node : mNodes) {
            wires.push_back(node.getWire());
        }
        mOpenCvWrapper->drawContours(
            mImageSegmentationProc, wires, -1, cNodesColor, cNodesThickness, OpenCvWrapper::LineTypes::LINE_8, {});

        mOpenCvWrapper->writeImage("image_segment_detect_nodes.png", mImageSegmentationProc);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Detecting nodes", mImageSegmentationProc, 0);
    }

    return true;
}

void ImageSegmentation::detectCircuitConnections()
{
    mLogger->logInfo("Detecting circuit connections");

    const auto imgWidth{mOpenCvWrapper->getImageWidth(mImagePreprocessed)};
    const auto imgHeight{mOpenCvWrapper->getImageHeight(mImagePreprocessed)};
    constexpr int widthIncr{2};  // 2 pixels to allow centering
    constexpr int heightIncr{2}; // 2 pixels to allow centering

    for (auto& component : mComponents) {
        // Increase dimensions of bounding box to allow intersection points with nodes
        const auto box = increaseBoundingBox(component.getBoundingBox(), widthIncr, heightIncr, imgWidth, imgHeight);

        std::vector<circuit::Node> nodesConnected{};

        mLogger->logDebug("Checking component with top-left corner (" + std::to_string(component.getBoundingBox().x)
                          + ", " + std::to_string(component.getBoundingBox().y) + ")");

        for (const auto& node : mNodes) {
            const auto wire{node.getWire()};
            auto intersect{false};

            for (const auto& point : wire) {
                // Intersection
                intersect = mOpenCvWrapper->contains(box, point);
                if (intersect) {
                    mLogger->logDebug("Component connected to a node wire at point (" + std::to_string(point.x) + ", "
                                      + std::to_string(point.y) + ")");

                    // There is at least one intersection point, so no need to check other points
                    break;
                }
            }

            if (intersect) {
                // Add node
                nodesConnected.push_back(node);
            }
        }

        // Set nodes to the component
        component.setNodes(nodesConnected);
    }
}

bool ImageSegmentation::updateDetectedComponents()
{
    mLogger->logInfo("Updating list of detected components");

    // Verify that the component is not connected to a node
    const auto isComponentNotConnected = [](circuit::Component component) {
        const auto nodes{component.getNodes()};
        return nodes.empty();
    };

    // Erase components from the vector that are not connected to a node
    mComponents.erase(std::remove_if(mComponents.begin(), mComponents.end(), isComponentNotConnected),
                      mComponents.end());

    mLogger->logInfo("Detected components in the image: " + std::to_string(mComponents.size()));

    // Return if there are still components in the vector
    return !mComponents.empty();
}

const std::vector<circuit::Component>& ImageSegmentation::getDetectedComponents() const
{
    return mComponents;
}

#ifdef BUILD_TESTS
void ImageSegmentation::setDetectedComponents(const std::vector<circuit::Component>& components)
{
    mComponents = components;
}

const std::vector<circuit::Node>& ImageSegmentation::getDetectedNodes() const
{
    return mNodes;
}

void ImageSegmentation::setDetectedNodes(const std::vector<circuit::Node>& nodes)
{
    mNodes = nodes;
}
#endif

} // namespace imageProcessing
} // namespace circuitSegmentation
