/**
 * @file
 */

#pragma once

#include "ImageProcUtils.h"
#include "logging/Logger.h"
#include <memory>
#include <opencv2/core.hpp>

namespace circuitSegmentation {
namespace imageProcessing {

/**
 * @brief Image preprocessing.
 */
class ImagePreprocessing
{
public:
    /** Maximum dimension of the image (width or height). */
    static constexpr int cResizeDim{800};

    /**
     * @brief Constructor of a new object.
     *
     * @param imageProcUtils Image processing utils.
     * @param logger Logger.
     * @param saveImages Save images obtained during the processing.
     */
    ImagePreprocessing(std::shared_ptr<ImageProcUtils> imageProcUtils,
                       std::shared_ptr<logging::Logger> logger,
                       bool saveImages = false);

    /**
     * @brief Destructor of the object.
     */
    virtual ~ImagePreprocessing() = default;

    /**
     * @brief Preprocesses the image.
     *
     * @param image Image for preprocessing.
     */
    virtual void preprocessImage(cv::Mat& image);

    /**
     * @brief Resizes the image.
     *
     * @param image Image to resize.
     */
    virtual void resizeImage(cv::Mat& image);

    /**
     * @brief Converts the image to grayscale.
     *
     * @param image Image to convert.
     */
    virtual void convertImageToGray(cv::Mat& image);

    /**
     * @brief Blurs the image.
     *
     * @param image Image to blur.
     */
    virtual void blurImage(cv::Mat& image);

    /**
     * @brief Detects edges the image.
     *
     * @param image Image to detect edges.
     */
    virtual void edgesImage(cv::Mat& image);

    /**
     * @brief Sets the flag to save images obtained during the processing.
     *
     * @param saveImages Save images obtained during the processing.
     */
    virtual void setSaveImages(bool saveImages);

    /**
     * @brief Gets the flag to save images obtained during the processing.
     *
     * @return Returns the flag to save images obtained during the processing.
     */
    [[nodiscard]] virtual bool getSaveImages() const;

private:
    /** Size of the kernel for filter (must be odd and positive). */
    const unsigned int cFilterKernelSize{3};

    /** Threshold1 value for the Canny Edge Detector. */
    const double cCannyEdgeThresh1{50};
    /** Threshold2 value for the Canny Edge Detector. */
    const double cCannyEdgeThresh2{200};
    /** Aperture size for the Sobel operator, for the Canny Edge Detector. */
    const int cCannyEdgeApertureSize{3};

    /** Image processing utils. */
    std::shared_ptr<ImageProcUtils> mImageProcUtils;

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Flag to save images obtained during the processing in the working directory. */
    bool mSaveImages{false};
};

} // namespace imageProcessing
} // namespace circuitSegmentation
