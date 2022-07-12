/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include "logging/Logger.h"
#include "schematicSegmentation/ComponentSegmentation.h"
#include "schematicSegmentation/ConnectionSegmentation.h"
#include <memory>

namespace circuitSegmentation {
namespace imageProcessing {

/**
 * @brief Image segmentation.
 */
class ImageSegmentation
{
public:
    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     * @param componentSegmentation Component segmentation.
     * @param connectionSegmentation Connection segmentation.
     * @param saveImages Save images obtained during the processing.
     */
    explicit ImageSegmentation(
        const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
        const std::shared_ptr<logging::Logger>& logger,
        const std::shared_ptr<schematicSegmentation::ComponentSegmentation>& componentSegmentation,
        const std::shared_ptr<schematicSegmentation::ConnectionSegmentation>& connectionSegmentation,
        const bool saveImages = false);

    /**
     * @brief Destructor.
     */
    virtual ~ImageSegmentation() = default;

    /**
     * @brief Segments the image.
     *
     * @param imageInitial Initial image without preprocessing.
     * @param imagePreprocessed Image preprocessed for segmentation.
     *
     * @return True if segmentation occurred successfully, otherwise false.
     */
    virtual bool segmentImage(computerVision::ImageMat imageInitial, computerVision::ImageMat imagePreprocessed);

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
    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Component segmentation. */
    std::shared_ptr<schematicSegmentation::ComponentSegmentation> mComponentSegmentation;

    /** Connection segmentation. */
    std::shared_ptr<schematicSegmentation::ConnectionSegmentation> mConnectionSegmentation;

    /** Flag to save images obtained during the processing in the working directory. */
    bool mSaveImages{false};
};

} // namespace imageProcessing
} // namespace circuitSegmentation
