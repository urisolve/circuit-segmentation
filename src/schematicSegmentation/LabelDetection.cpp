/**
 * @file
 */

#include "LabelDetection.h"
#include "SegmentationUtils.h"

namespace circuitSegmentation {
namespace schematicSegmentation {

LabelDetection::LabelDetection(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                               const std::shared_ptr<logging::Logger>& logger)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
    , mLabels{}
{
}

bool LabelDetection::detectLabels(computerVision::ImageMat& imageInitial,
                                  computerVision::ImageMat& imagePreprocessed,
                                  const std::vector<circuit::Component>& components,
                                  const std::vector<circuit::Connection>& connections,
                                  const bool saveImages)
{
    /*
     * Detection of labels
     * - Remove the elements from the preprocessed image (set elements with black pixels)
     * - Morphological closing for dilation of labels (this is useful to join all letters/words and digits)
     * - Morphological opening to remove the circuit connections (it can have the connections with nodes, because during
     * the detection of nodes and update of connections, the contours are not the same as the image)
     * - Find contours in the image after dilation of labels
     * - For each contour:
     *      - Generate a bounding box
     *      - For each bounding box:
     *          - Check bounding box area
     *          - If the bounding box has the minimum area, save it and consider as a label
     */

    mLogger->logInfo("Detecting labels");

    // Image used during the process
    computerVision::ImageMat image{mOpenCvWrapper->cloneImage(imagePreprocessed)};

    // Remove the elements from the preprocessed image (set elements with black pixels)
    removeElementsFromImage(image, components, connections);

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("cs_segment_labels_remove_elements.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Removing the elements from the preprocessed image", image, 0);
    }

    // Morphological closing for dilation of labels
    auto kernelMorph{mOpenCvWrapper->getStructuringElement(computerVision::OpenCvWrapper::MorphShapes::MORPH_RECT,
                                                           cMorphCloseKernelSize)};
    mOpenCvWrapper->morphologyEx(
        image, image, computerVision::OpenCvWrapper::MorphTypes::MORPH_CLOSE, kernelMorph, cMorphCloseIter);

    mLogger->logInfo("Morphological closing applied to the image");

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("cs_segment_labels_morph_close.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological closing to detect labels", image, 0);
    }

    // Morphological opening to remove the circuit connections
    kernelMorph = mOpenCvWrapper->getStructuringElement(computerVision::OpenCvWrapper::MorphShapes::MORPH_RECT,
                                                        cMorphOpenKernelSize);
    mOpenCvWrapper->morphologyEx(
        image, image, computerVision::OpenCvWrapper::MorphTypes::MORPH_OPEN, kernelMorph, cMorphOpenIter);

    mLogger->logInfo("Morphological opening applied to the image");

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("cs_segment_labels_morph_open.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological opening to detect labels", image, 0);
    }

    // At this point, the labels are in the image, so we need to find the contours
    computerVision::Contours contours{};
    computerVision::ContoursHierarchy hierarchy{};
    mOpenCvWrapper->findContours(image, contours, hierarchy, cFindContourMode, cFindContourMethod);

    mLogger->logDebug("Contours found in the image, to detect labels: " + std::to_string(contours.size()));

    for (const auto& contour : contours) {
        // Check contour
        const auto box{checkContour(imagePreprocessed, contour)};

        if (box.has_value()) {
            // Add label
            circuit::Label label{};
            label.mBoundingBox = box.value();
            mLabels.push_back(label);
        }
    }

    mLogger->logInfo("Labels found in the circuit: " + std::to_string(mLabels.size()));

    // If there are no labels detected, it makes no sense to continue
    if (mLabels.empty()) {
        return false;
    }

    // Save image
    if (saveImages) {
        image = mOpenCvWrapper->cloneImage(imageInitial);
        for (const auto& label : mLabels) {
            mOpenCvWrapper->rectangle(
                image, label.mBoundingBox, cBoxColor, cBoxThickness, computerVision::OpenCvWrapper::LineTypes::LINE_8);
        }

        mOpenCvWrapper->writeImage("cs_segment_labels_detected.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Detecting labels", image, 0);
    }

    return true;
}

const std::vector<circuit::Label>& LabelDetection::getDetectedLabels() const
{
    return mLabels;
}

void LabelDetection::removeElementsFromImage(computerVision::ImageMat& image,
                                             const std::vector<circuit::Component>& components,
                                             const std::vector<circuit::Connection>& connections)
{
    // Remove components
    for (const auto& component : components) {
        mOpenCvWrapper->rectangle(
            image, component.mBoundingBox, {0, 0, 0}, -1, computerVision::OpenCvWrapper::LineTypes::LINE_8);
    }

    // Remove connections
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
    LabelDetection::checkContour(computerVision::ImageMat& imagePreprocessed, const computerVision::Contour& contour)
{
    constexpr int widthIncr{2};  // 2 pixels to allow centering
    constexpr int heightIncr{2}; // 2 pixels to allow centering

    // Bounding box
    const auto box{generateBoundingBox(mOpenCvWrapper, contour, imagePreprocessed, widthIncr, heightIncr)};

    auto intersect{false};

    // Check bounding box area
    if (mOpenCvWrapper->rectangleArea(box) >= cBoxMinArea) {
        return box;
    }

    return std::nullopt;
}

} // namespace schematicSegmentation
} // namespace circuitSegmentation
