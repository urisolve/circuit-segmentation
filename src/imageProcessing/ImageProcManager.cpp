/**
 * @file
 */

#include "ImageProcManager.h"

namespace circuitSegmentation {
namespace imageProcessing {

ImageProcManager::ImageProcManager(const std::shared_ptr<ImageReceiver>& imageReceiver,
                                   const std::shared_ptr<ImagePreprocessing>& imagePreprocessing,
                                   const std::shared_ptr<ImageSegmentation>& imageSegmentation,
                                   const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                   const std::shared_ptr<logging::Logger>& logger,
                                   const bool logMode,
                                   const bool saveImages)
    : mImageReceiver{imageReceiver}
    , mImagePreprocessing{imagePreprocessing}
    , mImageSegmentation{imageSegmentation}
    , mOpenCvWrapper{openCvWrapper}
    , mLogger{logger}
    , mLogMode{std::move(logMode)}
    , mSaveImages{std::move(saveImages)}
{
    // Log mode
    setLogMode(mLogMode);

    // Save images
    setSaveImages(mSaveImages);
}

ImageProcManager
    ImageProcManager::create(const std::shared_ptr<logging::Logger>& logger, const bool logMode, const bool saveImages)
{
    std::shared_ptr<computerVision::OpenCvWrapper> openCvWrapper{std::make_shared<computerVision::OpenCvWrapper>()};

    return ImageProcManager(std::make_shared<ImageReceiver>(openCvWrapper, logger),
                            std::make_shared<ImagePreprocessing>(openCvWrapper, logger),
                            std::make_shared<ImageSegmentation>(openCvWrapper, logger),
                            openCvWrapper,
                            logger,
                            logMode,
                            saveImages);
}

bool ImageProcManager::processImage(const std::string imageFilePath)
{
    mLogger->logInfo("Starting image processing");

    // Receive image
    if (!receiveImage(imageFilePath)) {
        mLogger->logInfo("Failed during image reception");
        return false;
    }
    mLogger->logInfo("Image received successfully");

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_initial.png", mImageInitial);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Initial image", mImageInitial, 0);
    }

    // Preprocessing
    preprocessImage();
    mLogger->logInfo("Image preprocessing occurred successfully");

    // Segmentation
    if (!segmentImage()) {
        mLogger->logInfo("Failed during image segmentation");
        return false;
    }
    mLogger->logInfo("Image segmentation occurred successfully");

    // TODO: Segmentation map.

    // TODO: Images (ROI) of components with ID (ID in the file name).

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
    mImageSegmentation->setSaveImages(mSaveImages);
}

bool ImageProcManager::getSaveImages() const
{
    return mSaveImages;
}

bool ImageProcManager::receiveImage(const std::string& filePath)
{
    // Set image file path
    mImageReceiver->setImageFilePath(filePath);

    // Receive image from image receiver
    if (!mImageReceiver->receiveImage()) {
        return false;
    }

    // Get image received
    mImageInitial = mImageReceiver->getImageReceived();

    return true;
}

void ImageProcManager::preprocessImage()
{
    // Copy initial image
    mImageProcessed = mOpenCvWrapper->cloneImage(mImageInitial);

    // Preprocess the image
    mImagePreprocessing->preprocessImage(mImageProcessed);
}

bool ImageProcManager::segmentImage()
{
    // Segment the image
    return mImageSegmentation->segmentImage(mImageInitial, mImageProcessed);
}

} // namespace imageProcessing
} // namespace circuitSegmentation
