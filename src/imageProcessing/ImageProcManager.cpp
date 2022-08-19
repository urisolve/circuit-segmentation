/**
 * @file
 */

#include "ImageProcManager.h"
#include "application/Config.h"
#include "schematicSegmentation/ComponentDetection.h"
#include "schematicSegmentation/ConnectionDetection.h"
#include "schematicSegmentation/LabelDetection.h"

namespace circuitSegmentation {
namespace imageProcessing {

ImageProcManager::ImageProcManager(
    const std::shared_ptr<ImageReceiver>& imageReceiver,
    const std::shared_ptr<ImagePreprocessing>& imagePreprocessing,
    const std::shared_ptr<ImageSegmentation>& imageSegmentation,
    const std::shared_ptr<schematicSegmentation::SchematicSegmentation>& schematicSegmentation,
    const std::shared_ptr<schematicSegmentation::RoiSegmentation>& roiSegmentation,
    const std::shared_ptr<schematicSegmentation::SegmentationMap>& segmentationMap,
    const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
    const std::shared_ptr<logging::Logger>& logger,
    const bool logMode,
    const bool saveImages)
    : mImageReceiver{imageReceiver}
    , mImagePreprocessing{imagePreprocessing}
    , mImageSegmentation{imageSegmentation}
    , mSchematicSegmentation{schematicSegmentation}
    , mRoiSegmentation{roiSegmentation}
    , mSegmentationMap{segmentationMap}
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
    std::shared_ptr<schematicSegmentation::ComponentDetection> componentDetection{
        std::make_shared<schematicSegmentation::ComponentDetection>(openCvWrapper, logger)};
    std::shared_ptr<schematicSegmentation::ConnectionDetection> connectionDetection{
        std::make_shared<schematicSegmentation::ConnectionDetection>(openCvWrapper, logger)};
    std::shared_ptr<schematicSegmentation::LabelDetection> labelDetection{
        std::make_shared<schematicSegmentation::LabelDetection>(openCvWrapper, logger)};
    std::shared_ptr<schematicSegmentation::SchematicSegmentation> schematicSegmentation{
        std::make_shared<schematicSegmentation::SchematicSegmentation>(openCvWrapper, logger)};

    return ImageProcManager(
        std::make_shared<ImageReceiver>(openCvWrapper, logger),
        std::make_shared<ImagePreprocessing>(openCvWrapper, logger),
        std::make_shared<ImageSegmentation>(
            openCvWrapper, logger, componentDetection, connectionDetection, labelDetection, schematicSegmentation),
        schematicSegmentation,
        std::make_shared<schematicSegmentation::RoiSegmentation>(openCvWrapper, logger),
        std::make_shared<schematicSegmentation::SegmentationMap>(logger),
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
        mLogger->logError("Failed during image reception");
        return false;
    }
    mLogger->logInfo("Image received successfully");

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("cs_initial_image.png", mImageInitial);
#ifdef SHOW_IMAGES
        mOpenCvWrapper->showImage("Initial image", mImageInitial, 0);
#endif
    }

    // Preprocessing
    preprocessImage();
    mLogger->logInfo("Image preprocessing occurred successfully");

    // Segmentation
    if (!segmentImage()) {
        mLogger->logError("Failed during image segmentation");
        return false;
    }
    mLogger->logInfo("Image segmentation occurred successfully");

    // Images with regions of interest (ROI) for components and labels
    if (!generateImageRoi()) {
        mLogger->logError("Failed during generation of images with ROI");
        return false;
    }
    mLogger->logInfo("Generation of images with ROI occurred successfully");

    // Segmentation map
    if (!generateSegmentationMap()) {
        mLogger->logError("Failed during generation of segmentation map file");
        return false;
    }
    mLogger->logInfo("Generation of segmentation map file occurred successfully");

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

bool ImageProcManager::generateImageRoi()
{
    // Generate images with (ROI) for components
    if (!mRoiSegmentation->generateRoiComponents(mImageInitial, mSchematicSegmentation->getComponents())) {
        return false;
    }

    // Generate images with (ROI) for labels
    if (!mRoiSegmentation->generateRoiLabels(mImageInitial,
                                             mSchematicSegmentation->getComponents(),
                                             mSchematicSegmentation->getConnections(),
                                             mSchematicSegmentation->getNodes())) {
        return false;
    }

    return true;
}

bool ImageProcManager::generateSegmentationMap()
{
    // Generate segmentation map
    if (!mSegmentationMap->generateSegmentationMap(mSchematicSegmentation->getComponents(),
                                                   mSchematicSegmentation->getConnections(),
                                                   mSchematicSegmentation->getNodes())) {
        return false;
    }

    // Write segmentation map file
    if (!mSegmentationMap->writeSegmentationMapJsonFile()) {
        return false;
    }

    return true;
}

} // namespace imageProcessing
} // namespace circuitSegmentation
