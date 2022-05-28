/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include "ImagePreprocessing.h"
#include "ImageReceiver.h"
#include "logging/Logger.h"
#include <memory>
#include <string>

namespace circuitSegmentation {
namespace imageProcessing {

/**
 * @brief Image processing manager.
 */
class ImageProcManager
{
public:
    /**
     * @brief Constructor.
     *
     * @param imageReceiver Image receiver.
     * @param imagePreprocessing Image preprocessing.
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     * @param logMode Log mode: verbose = true, silent = false.
     * @param saveImages Save images obtained during the processing.
     */
    ImageProcManager(std::shared_ptr<ImageReceiver> imageReceiver,
                     std::shared_ptr<ImagePreprocessing> imagePreprocessing,
                     std::shared_ptr<computerVision::OpenCvWrapper> openCvWrapper,
                     std::shared_ptr<logging::Logger> logger,
                     bool logMode = false,
                     bool saveImages = false);

    /**
     * @brief Destructor.
     */
    virtual ~ImageProcManager() = default;

    /**
     * @brief Creates an image processing manager.
     *
     * @param logger Logger.
     * @param logMode Log mode: verbose = true, silent = false.
     * @param saveImages Save images obtained during the processing.
     *
     * @return An instance of an image processing manager.
     */
    static ImageProcManager
        create(std::shared_ptr<logging::Logger> logger, bool logMode = false, bool saveImages = false);

    /**
     * @brief Processes the image.
     *
     * This method performs the following:
     * - Preprocessing of the image
     * - Segmentation of the image
     *
     * @param imageFilePath Image file path for processing.
     *
     * @return True if the processing terminated successfully, otherwise false.
     */
    virtual bool processImage(const std::string imageFilePath);

    /**
     * @brief Sets the log mode.
     *
     * @param logMode Log mode: verbose = true, silent = false.
     */
    virtual void setLogMode(const bool& logMode);

    /**
     * @brief Gets the log mode setted.
     *
     * @return The log mode: verbose = true, silent = false.
     */
    [[nodiscard]] virtual bool getLogMode() const;

    /**
     * @brief Sets the flag to save images obtained during the processing.
     *
     * @param saveImages Save images obtained during the processing.
     */
    virtual void setSaveImages(const bool& saveImages);

    /**
     * @brief Gets the flag to save images obtained during the processing.
     *
     * @return The flag to save images obtained during the processing.
     */
    [[nodiscard]] virtual bool getSaveImages() const;

private:
    /** Image receiver. */
    std::shared_ptr<ImageReceiver> mImageReceiver;
    /** Original image for processing. */
    computerVision::ImageMat mImageOriginal{};

    /** Image preprocessing. */
    std::shared_ptr<ImagePreprocessing> mImagePreprocessing;

    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;

    /** Processed image. */
    computerVision::ImageMat mImageProcessed{};

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Log mode: verbose = true, silent = false. */
    bool mLogMode{false};
    /** Flag to save images obtained during the processing in the working directory. */
    bool mSaveImages{false};

    /**
     * @brief Receives the original image for processing.
     *
     * @param filePath Image file path.
     *
     * @return True if image is okay, otherwise false.
     */
    virtual bool receiveOriginalImage(const std::string& filePath);

    /**
     * @brief Preprocesses the image.
     */
    virtual void preprocessImage();
};

} // namespace imageProcessing
} // namespace circuitSegmentation
