/**
 * @file
 */

#include "ImageReceiver.h"
#include <opencv2/imgcodecs.hpp>

using namespace circuitSegmentation::imageProcessing;

ImageReceiver::ImageReceiver(std::shared_ptr<logging::Logger> logger)
    : mLogger{std::move(logger)}
{
}

bool ImageReceiver::receiveImage()
{
    // Read image from file
    mImage = cv::imread(mImageFilePath, cv::IMREAD_COLOR);

    // Check image
    if (mImage.empty()) {
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
