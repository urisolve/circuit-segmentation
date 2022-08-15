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
    const std::shared_ptr<schematicSegmentation::LabelDetection>& labelDetection,
    const std::shared_ptr<schematicSegmentation::SchematicSegmentation>& schematicSegmentation,
    const bool saveImages)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
    , mComponentDetection{componentDetection}
    , mConnectionDetection{connectionDetection}
    , mLabelDetection{labelDetection}
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

    // Detect labels
    if (mLabelDetection->detectLabels(imageInitial,
                                      imagePreprocessed,
                                      mSchematicSegmentation->getComponents(),
                                      mSchematicSegmentation->getConnections(),
                                      mSaveImages)) {
        // Associate labels
        mSchematicSegmentation->associateLabels(
            imageInitial, imagePreprocessed, mLabelDetection->getDetectedLabels(), mSaveImages);
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

} // namespace imageProcessing
} // namespace circuitSegmentation
