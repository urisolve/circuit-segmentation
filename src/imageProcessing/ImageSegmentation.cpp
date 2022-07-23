/**
 * @file
 */

#include "ImageSegmentation.h"

namespace circuitSegmentation {
namespace imageProcessing {

ImageSegmentation::ImageSegmentation(
    const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
    const std::shared_ptr<logging::Logger>& logger,
    const std::shared_ptr<schematicSegmentation::ComponentSegmentation>& componentSegmentation,
    const std::shared_ptr<schematicSegmentation::ConnectionSegmentation>& connectionSegmentation,
    const bool saveImages)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
    , mComponentSegmentation{componentSegmentation}
    , mConnectionSegmentation{connectionSegmentation}
    , mSaveImages{std::move(saveImages)}
{
}

bool ImageSegmentation::segmentImage(computerVision::ImageMat imageInitial, computerVision::ImageMat imagePreprocessed)
{
    mLogger->logInfo("Starting image segmentation");

    // Detect components
    if (!mComponentSegmentation->detectComponents(imageInitial, imagePreprocessed, mSaveImages)) {
        return false;
    }

    // Detect connections
    if (!mConnectionSegmentation->detectConnections(
            imageInitial, imagePreprocessed, mComponentSegmentation->getDetectedComponents(), mSaveImages)) {
        return false;
    }

    // Detect component connections
    mComponentSegmentation->detectComponentConnections(imagePreprocessed,
                                                       mConnectionSegmentation->getDetectedConnections());

    // Update list of detected components
    if (!mComponentSegmentation->updateDetectedComponents()) {
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

} // namespace imageProcessing
} // namespace circuitSegmentation
