/**
 * @file
 */

#include "ImageReceiver.h"

namespace circuitSegmentation {
namespace imageProcessing {

ImageReceiver::ImageReceiver(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                             const std::shared_ptr<logging::Logger>& logger)
    : mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
{
}

bool ImageReceiver::receiveImage()
{
    // Read image from file
    mImage = mOpenCvWrapper->readImage(mImageFilePath);

    // Check image
    if (mOpenCvWrapper->isImageEmpty(mImage)) {
        mLogger->logWarning("Image cannot be open/read with path: " + mImageFilePath);
        return false;
    }
    mLogger->logInfo("Image file path: " + mImageFilePath);

    return true;
}

[[nodiscard]] cv::Mat ImageReceiver::getImageReceived() const
{
    return mImage;
}

void ImageReceiver::setImageFilePath(const std::string& filePath)
{
    mImageFilePath = filePath;
}

[[nodiscard]] std::string ImageReceiver::getImageFilePath() const
{
    return mImageFilePath;
}

} // namespace imageProcessing
} // namespace circuitSegmentation
