/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include "logging/Logger.h"
#include <memory>
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
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     */
    explicit ImageReceiver(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                           const std::shared_ptr<logging::Logger>& logger);

    /**
     * @brief Destructor.
     */
    virtual ~ImageReceiver() = default;

    /**
     * @brief Receives the image for processing.
     *
     * @return True if image is okay, otherwise false when the image cannot be read because of missing file, improper
     * permissions, unsupported or invalid format.
     */
    virtual bool receiveImage();

    /**
     * @brief Gets the image received for processing.
     *
     * @return Image received.
     */
    [[nodiscard]] virtual computerVision::ImageMat getImageReceived() const;

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
    computerVision::ImageMat mImage{};

    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;
    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;
};

} // namespace imageProcessing
} // namespace circuitSegmentation
