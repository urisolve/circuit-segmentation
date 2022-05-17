/**
 * @file
 */

#include "ImageProcManager.h"
#include <opencv2/core/utils/logger.hpp>

using namespace circuitSegmentation::imageProcessing;

ImageProcManager::ImageProcManager(std::shared_ptr<ImageReceiver> imageReceiver,
                                   std::shared_ptr<ImagePreprocessing> imagePreprocessing,
                                   std::shared_ptr<ImageProcUtils> imageProcUtils,
                                   std::shared_ptr<logging::Logger> logger,
                                   bool logMode,
                                   bool saveImages)
    : mImageReceiver{imageReceiver}
    , mImagePreprocessing{imagePreprocessing}
    , mImageProcUtils{imageProcUtils}
    , mLogger{logger}
    , mLogMode{logMode}
    , mSaveImages{saveImages}
{
    // Log mode
    setLogMode(mLogMode);

    // Save images
    setSaveImages(mSaveImages);
}

ImageProcManager ImageProcManager::create(std::shared_ptr<logging::Logger> logger, bool logMode, bool saveImages)
{
    std::shared_ptr<ImageProcUtils> imageProcUtils{std::make_shared<ImageProcUtils>()};

    return ImageProcManager(std::make_shared<ImageReceiver>(logger),
                            std::make_shared<ImagePreprocessing>(imageProcUtils, logger),
                            imageProcUtils,
                            logger,
                            logMode,
                            saveImages);
}

bool ImageProcManager::processImage(const std::string imageFilePath)
{
    // Receive original image
    if (!receiveOriginalImage(imageFilePath)) {
        return false;
    }

    // Save image
    if (mSaveImages) {
        mImageProcUtils->writeImage("image_original.png", mImageOriginal);
        // TODO: Remove or comment.
        mImageProcUtils->showImage("Original image", mImageOriginal, 0);
    }

    // Preprocessing
    preprocessImage();

    // Save image
    if (mSaveImages) {
        mImageProcUtils->writeImage("image_preprocessed.png", mImageProcessed);
        // TODO: Remove or comment.
        mImageProcUtils->showImage("Preprocessed image", mImageProcessed, 0);
    }

    // TODO: Segmentation.

    // TODO: Segmentation map.

    return true;
}

bool ImageProcManager::receiveOriginalImage(const std::string& filePath)
{
    // Set image file path
    mImageReceiver->setImageFilePath(filePath);

    // Receive image from image receiver
    if (!mImageReceiver->receiveImage()) {
        return false;
    }

    // Get image received
    mImageOriginal = mImageReceiver->getImageReceived();

    return true;
}

void ImageProcManager::preprocessImage()
{
    // Copy original image
    mImageProcessed = mImageOriginal.clone();

    // Preprocess the image
    mImagePreprocessing->preprocessImage(mImageProcessed);
}

void ImageProcManager::setLogMode(bool logMode)
{
    mLogMode = logMode;

    if (mLogMode) {
        // Enable verbose logs
        mLogger->setLogLevel(logging::Logger::LogLevel::VERBOSE);

        // Set OpenCV log level to Warning
        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
    } else {
        // Disable logs
        mLogger->setLogLevel(logging::Logger::LogLevel::NONE);

        // Set OpenCV log level to Silent
        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    }
}

bool ImageProcManager::getLogMode() const
{
    return mLogMode;
}

void ImageProcManager::setSaveImages(bool saveImages)
{
    mSaveImages = saveImages;

    mImagePreprocessing->setSaveImages(mSaveImages);
}

bool ImageProcManager::getSaveImages() const
{
    return mSaveImages;
}
