/**
 * @file
 */

#include "ImageProcManager.h"

namespace circuitSegmentation {
namespace imageProcessing {

ImageProcManager::ImageProcManager(std::shared_ptr<ImageReceiver> imageReceiver,
                                   std::shared_ptr<ImagePreprocessing> imagePreprocessing,
                                   std::shared_ptr<ImageSegmentation> imageSegmentation,
                                   std::shared_ptr<computerVision::OpenCvWrapper> openCvWrapper,
                                   std::shared_ptr<logging::Logger> logger,
                                   bool logMode,
                                   bool saveImages)
    : mImageReceiver{std::move(imageReceiver)}
    , mImagePreprocessing{std::move(imagePreprocessing)}
    , mImageSegmentation{std::move(imageSegmentation)}
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
                            std::make_shared<ImageSegmentation>(openCvWrapper, logger),
                            openCvWrapper,
                            logger,
                            logMode,
                            saveImages);
}

bool ImageProcManager::processImage(const std::string imageFilePath)
{
    // Receive image
    if (!receiveImage(imageFilePath)) {
        return false;
    }

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_initial.png", mImageInitial);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Initial image", mImageInitial, 0);
    }

    // Preprocessing
    preprocessImage();

    // Segmentation.
    if (!segmentImage()) {
        return false;
    }

    // TODO: Segmentation map.

    // TODO: Images of components with ID.

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
