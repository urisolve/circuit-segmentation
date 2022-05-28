/**
 * @file
 */

#include "ImageReceiver.h"

using namespace circuitSegmentation::imageProcessing;

ImageReceiver::ImageReceiver(std::shared_ptr<computerVision::OpenCvWrapper> openCvWrapper,
                             std::shared_ptr<logging::Logger> logger)
    : mOpenCvWrapper{std::move(openCvWrapper)}
    , mLogger{std::move(logger)}
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
    mLogger->logInfo("Image okay with path: " + mImageFilePath);

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
