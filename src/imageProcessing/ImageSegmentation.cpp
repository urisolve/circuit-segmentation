/**
 * @file
 */

#include "ImageSegmentation.h"

namespace circuitSegmentation {
namespace imageProcessing {

ImageSegmentation::ImageSegmentation(
    const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
    const std::shared_ptr<logging::Logger>& logger,
    const std::shared_ptr<schematicSegmentation::ComponentDetection>& componentDetection,
    const std::shared_ptr<schematicSegmentation::ConnectionDetection>& connectionDetection,
    const std::shared_ptr<schematicSegmentation::SchematicSegmentation>& schematicSegmentation,
    const bool saveImages)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
    , mComponentDetection{componentDetection}
    , mConnectionDetection{connectionDetection}
    , mSchematicSegmentation{schematicSegmentation}
    , mSaveImages{std::move(saveImages)}
{
}

bool ImageSegmentation::segmentImage(computerVision::ImageMat imageInitial, computerVision::ImageMat imagePreprocessed)
{
    mLogger->logInfo("Starting image segmentation");

    // Detect connections
    if (!mConnectionDetection->detectConnections(imageInitial, imagePreprocessed, mSaveImages)) {
        return false;
    }

    // Detect components
    if (!mComponentDetection->detectComponents(
            imageInitial, imagePreprocessed, mConnectionDetection->getDetectedConnections(), mSaveImages)) {
        return false;
    }

    // Update connections
    if (!mConnectionDetection->updateConnections(
            imageInitial, imagePreprocessed, mComponentDetection->getDetectedComponents(), mSaveImages)) {
        return false;
    }

    // Detect nodes and update connections
    if (!mConnectionDetection->detectNodesUpdateConnections(
            imageInitial, imagePreprocessed, mComponentDetection->getDetectedComponents(), mSaveImages)) {
        return false;
    }

    // Detect component connections
    mSchematicSegmentation->detectComponentConnections(imageInitial,
                                                       imagePreprocessed,
                                                       mComponentDetection->getDetectedComponents(),
                                                       mConnectionDetection->getDetectedConnections(),
                                                       mConnectionDetection->getDetectedNodes(),
                                                       mSaveImages);

    // Update list of detected components
    if (!mSchematicSegmentation->updateDetectedComponents()) {
        return false;
    }

    // TODO: Label segmentation.
    /*
    - Detect labels
        - In LabelDetection class
        - Remove detected elements of the image
        - The rest is considered as a label
        - Check smaller distance between label and element to detect the owner
    - Update labels of components and connections
        - In SchematicSegmentation class
    */

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

} // namespace imageProcessing
} // namespace circuitSegmentation
