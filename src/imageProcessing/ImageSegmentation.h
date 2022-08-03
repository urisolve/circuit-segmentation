/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include "logging/Logger.h"
#include "schematicSegmentation/ComponentDetection.h"
#include "schematicSegmentation/ConnectionDetection.h"
#include "schematicSegmentation/SchematicSegmentation.h"
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
     * @param componentDetection Component detection.
     * @param connectionDetection Connection detection.
     * @param schematicSegmentation Schematic segmentation.
     * @param saveImages Save images obtained during the processing.
     */
    explicit ImageSegmentation(
        const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
        const std::shared_ptr<logging::Logger>& logger,
        const std::shared_ptr<schematicSegmentation::ComponentDetection>& componentDetection,
        const std::shared_ptr<schematicSegmentation::ConnectionDetection>& connectionDetection,
        const std::shared_ptr<schematicSegmentation::SchematicSegmentation>& schematicSegmentation,
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

    /** Component detection. */
    std::shared_ptr<schematicSegmentation::ComponentDetection> mComponentDetection;

    /** Connection detection. */
    std::shared_ptr<schematicSegmentation::ConnectionDetection> mConnectionDetection;

    /** Schematic segmentation. */
    std::shared_ptr<schematicSegmentation::SchematicSegmentation> mSchematicSegmentation;

    /** Flag to save images obtained during the processing in the working directory. */
    bool mSaveImages{false};
};

} // namespace imageProcessing
} // namespace circuitSegmentation
