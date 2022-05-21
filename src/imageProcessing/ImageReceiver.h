/**
 * @file
 */

#pragma once

#include "logging/Logger.h"
#include <memory>
#include <opencv2/core.hpp>
#include <string>

namespace circuitSegmentation {
namespace imageProcessing {

/**
 * @brief Image receiver for processing.
 */
class ImageReceiver
{
public:
    /**
     * @brief Constructor.
     *
     * @param logger Logger.
     */
    ImageReceiver(std::shared_ptr<logging::Logger> logger);

    /**
     * @brief Destructor.
     */
    virtual ~ImageReceiver() = default;

    /**
     * @brief Receives the image for processing.
     *
     * @return Returns true if image is okay, otherwise false when the image cannot be read because of missing file,
     * improper permissions, unsupported or invalid format.
     */
    virtual bool receiveImage();

    /**
     * @brief Gets the image received for processing.
     *
     * @return Image received.
     */
    [[nodiscard]] virtual cv::Mat getImageReceived() const;

    /**
     * @brief Sets the image file path for processing.
     *
     * @param filePath File path of the image.
     */
    virtual void setImageFilePath(const std::string& filePath);

    /**
     * @brief Gets the image file path for processing.
     *
     * @return Image file path.
     */
    [[nodiscard]] virtual std::string getImageFilePath() const;

private:
    /** Image file path. */
    std::string mImageFilePath{};
    /** Image for processing. */
    cv::Mat mImage{};

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
};

} // namespace imageProcessing
} // namespace circuitSegmentation
