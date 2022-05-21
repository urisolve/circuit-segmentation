/**
 * @file
 */

#include "ImageProcManager.h"

using namespace circuitSegmentation::imageProcessing;

ImageProcManager::ImageProcManager(std::shared_ptr<ImageReceiver> imageReceiver,
                                   std::shared_ptr<ImagePreprocessing> imagePreprocessing,
                                   std::shared_ptr<computerVision::OpenCvWrapper> openCvWrapper,
                                   std::shared_ptr<logging::Logger> logger,
                                   bool logMode,
                                   bool saveImages)
    : mImageReceiver{std::move(imageReceiver)}
    , mImagePreprocessing{std::move(imagePreprocessing)}
    , mOpenCvWrapper{std::move(openCvWrapper)}
    , mLogger{std::move(logger)}
    , mLogMode{std::move(logMode)}
    , mSaveImages{std::move(saveImages)}
{
    // Log mode
    setLogMode(mLogMode);

    // Save images
    setSaveImages(mSaveImages);
}

ImageProcManager ImageProcManager::create(std::shared_ptr<logging::Logger> logger, bool logMode, bool saveImages)
{
    std::shared_ptr<computerVision::OpenCvWrapper> openCvWrapper{std::make_shared<computerVision::OpenCvWrapper>()};

    return ImageProcManager(std::make_shared<ImageReceiver>(openCvWrapper, logger),
                            std::make_shared<ImagePreprocessing>(openCvWrapper, logger),
                            openCvWrapper,
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
        mOpenCvWrapper->writeImage("image_original.png", mImageOriginal);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Original image", mImageOriginal, 0);
    }

    // Preprocessing
    preprocessImage();

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_preprocessed.png", mImageProcessed);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Preprocessed image", mImageProcessed, 0);
    }

    // TODO: Segmentation.

    // TODO: Segmentation map.

    return true;
}

void ImageProcManager::setLogMode(const bool& logMode)
{
    mLogMode = logMode;

    if (mLogMode) {
        // Enable verbose logs
        mLogger->setLogLevel(logging::Logger::LogLevel::VERBOSE);

        mOpenCvWrapper->setLogMode(false);
    } else {
        // Disable logs
        mLogger->setLogLevel(logging::Logger::LogLevel::NONE);

        mOpenCvWrapper->setLogMode(true);
    }
}

bool ImageProcManager::getLogMode() const
{
    return mLogMode;
}

void ImageProcManager::setSaveImages(const bool& saveImages)
{
    mSaveImages = saveImages;

    mImagePreprocessing->setSaveImages(mSaveImages);
}

bool ImageProcManager::getSaveImages() const
{
    return mSaveImages;
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
    mImageProcessed = mOpenCvWrapper->cloneImage(mImageOriginal);

    // Preprocess the image
    mImagePreprocessing->preprocessImage(mImageProcessed);
}
