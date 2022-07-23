/**
 * @file
 */

#include "ComponentDetection.h"
#include "SegmentationUtils.h"
#include <algorithm>

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
    auto kernelMorph{mOpenCvWrapper->getStructuringElement(computerVision::OpenCvWrapper::MorphShapes::MORPH_RECT,
                                                           cMorphCloseKernelSize)};
    mOpenCvWrapper->morphologyEx(
        imagePreprocessed, image, computerVision::OpenCvWrapper::MorphTypes::MORPH_CLOSE, kernelMorph, cMorphCloseIter);

    mLogger->logInfo("Morphological closing applied to the image, to detect components");

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("image_segment_morph_close.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological closing to detect components", image, 0);
    }

    // Morphological opening to remove the rest of the circuit leaving only the dilated components
    kernelMorph = mOpenCvWrapper->getStructuringElement(computerVision::OpenCvWrapper::MorphShapes::MORPH_RECT,
                                                        cMorphOpenKernelSize);
    mOpenCvWrapper->morphologyEx(
        image, image, computerVision::OpenCvWrapper::MorphTypes::MORPH_OPEN, kernelMorph, cMorphOpenIter);

    mLogger->logInfo("Morphological opening applied to the image, to detect components");

    // Save image
    if (saveImages) {
        mOpenCvWrapper->writeImage("image_segment_morph_open.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological opening to detect components", image, 0);
    }

    // At this point, the components are represented as blobs in the image, so we need to find those blobs

    computerVision::Contours componentsBlobs{};
    computerVision::ContoursHierarchy hierarchy{};

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
                                      computerVision::OpenCvWrapper::LineTypes::LINE_8);
        }

        mOpenCvWrapper->writeImage("image_segment_detect_components.png", image);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Detecting components", image, 0);
    }

    return true;
}

const std::vector<circuit::Component>& ComponentDetection::getDetectedComponents() const
{
    return mComponents;
}

void ComponentDetection::boundingBoxComponents(const computerVision::Contours& blobs,
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

} // namespace schematicSegmentation
} // namespace circuitSegmentation
