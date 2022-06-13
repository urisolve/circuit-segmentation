/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include "ImagePreprocessing.h"
#include "ImageReceiver.h"
#include "ImageSegmentation.h"
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
     * @param imageSegmentation Image segmentation.
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     * @param logMode Log mode: verbose = true, silent = false.
     * @param saveImages Save images obtained during the processing.
     */
    ImageProcManager(const std::shared_ptr<ImageReceiver>& imageReceiver,
                     const std::shared_ptr<ImagePreprocessing>& imagePreprocessing,
                     const std::shared_ptr<ImageSegmentation>& imageSegmentation,
                     const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                     const std::shared_ptr<logging::Logger>& logger,
                     const bool logMode = false,
                     const bool saveImages = false);

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
    static ImageProcManager create(const std::shared_ptr<logging::Logger>& logger,
                                   const bool logMode = false,
                                   const bool saveImages = false);

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
    /**
     * @brief Receives the image for processing.
     *
     * @param filePath Image file path.
     *
     * @return True if image is okay, otherwise false.
     */
    virtual bool receiveImage(const std::string& filePath);

    /**
     * @brief Preprocesses the image.
     */
    virtual void preprocessImage();

    /**
     * @brief Segments the image.
     *
     * @return True if image segmentation occurred successfully, otherwise false.
     */
    virtual bool segmentImage();

private:
    /** Image receiver. */
    std::shared_ptr<ImageReceiver> mImageReceiver;
    /** Initial Image for processing. */
    computerVision::ImageMat mImageInitial{};

    /** Image preprocessing. */
    std::shared_ptr<ImagePreprocessing> mImagePreprocessing;

    /** Image segmentation. */
    std::shared_ptr<ImageSegmentation> mImageSegmentation;

    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Processed image. */
    computerVision::ImageMat mImageProcessed{};

    /** Log mode: verbose = true, silent = false. */
    bool mLogMode{false};
    /** Flag to save images obtained during the processing in the working directory. */
    bool mSaveImages{false};
};

} // namespace imageProcessing
} // namespace circuitSegmentation
