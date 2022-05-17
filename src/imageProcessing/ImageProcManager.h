/**
 * @file
 */

#pragma once

#include "ImagePreprocessing.h"
#include "ImageProcUtils.h"
#include "ImageReceiver.h"
#include "logging/Logger.h"
#include <memory>
#include <opencv2/core.hpp>
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
     * @brief Constructor of a new object.
     *
     * @param imageReceiver Image receiver.
     * @param imagePreprocessing Image preprocessing.
     * @param imageProcUtils Image processing utils.
     * @param logger Logger.
     * @param logMode Log mode: verbose = true, silent = false.
     * @param saveImages Save images obtained during the processing.
     */
    ImageProcManager(std::shared_ptr<ImageReceiver> imageReceiver,
                     std::shared_ptr<ImagePreprocessing> imagePreprocessing,
                     std::shared_ptr<ImageProcUtils> imageProcUtils,
                     std::shared_ptr<logging::Logger> logger,
                     bool logMode = false,
                     bool saveImages = false);

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
     * @return Returns true if the processing terminated successfully, otherwise false.
     */
    bool processImage(const std::string imageFilePath);

    /**
     * @brief Sets the log mode.
     *
     * @param logMode Log mode: verbose = true, silent = false.
     */
    void setLogMode(bool logMode);

    /**
     * @brief Gets the log mode setted.
     *
     * @return Returns the log mode: verbose = true, silent = false.
     */
    [[nodiscard]] bool getLogMode() const;

    /**
     * @brief Sets the flag to save images obtained during the processing.
     *
     * @param saveImages Save images obtained during the processing.
     */
    void setSaveImages(bool saveImages);

    /**
     * @brief Gets the flag to save images obtained during the processing.
     *
     * @return Returns the flag to save images obtained during the processing.
     */
    [[nodiscard]] bool getSaveImages() const;

private:
    /** Image receiver. */
    std::shared_ptr<ImageReceiver> mImageReceiver;
    /** Original image for processing. */
    cv::Mat mImageOriginal{};

    /** Image preprocessing. */
    std::shared_ptr<ImagePreprocessing> mImagePreprocessing;

    /** Image processing utils. */
    std::shared_ptr<ImageProcUtils> mImageProcUtils;

    /** Processed image. */
    cv::Mat mImageProcessed{};

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
     * @return Returns true if image is okay, otherwise false.
     */
    bool receiveOriginalImage(const std::string& filePath);

    /**
     * @brief Preprocesses the image.
     */
    void preprocessImage();
};

} // namespace imageProcessing
} // namespace circuitSegmentation
