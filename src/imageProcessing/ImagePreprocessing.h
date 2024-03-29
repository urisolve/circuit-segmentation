/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include "logging/Logger.h"
#include <memory>

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
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     * @param saveImages Save images obtained during the processing.
     */
    explicit ImagePreprocessing(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                                const std::shared_ptr<logging::Logger>& logger,
                                const bool saveImages = false);

    /**
     * @brief Destructor.
     */
    virtual ~ImagePreprocessing() = default;

    /**
     * @brief Preprocesses the image.
     *
     * @param image Image for preprocessing.
     */
    virtual void preprocessImage(computerVision::ImageMat& image);

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

#ifndef BUILD_TESTS
private:
#endif
    /**
     * @brief Resizes the image.
     *
     * @param image Image to resize.
     */
    virtual void resizeImage(computerVision::ImageMat& image);

    /**
     * @brief Converts the image to grayscale.
     *
     * @param image Image to convert.
     */
    virtual void convertImageToGray(computerVision::ImageMat& image);

    /**
     * @brief Blurs the image.
     *
     * @param image Image to blur.
     */
    virtual void blurImage(computerVision::ImageMat& image);

    /**
     * @brief Applies a threshold to the image.
     *
     * @param image Image to apply a threshold.
     */
    virtual void thresholdImage(computerVision::ImageMat& image);

    /**
     * @brief Applies a morphological opening to the image.
     *
     * @param image Image to apply a morphological opening.
     */
    virtual void morphologicalOpenImage(computerVision::ImageMat& image);

    /**
     * @brief Applies a morphological dilation to the image.
     *
     * @param image Image to apply a morphological dilation.
     */
    virtual void morphologicalDilateImage(computerVision::ImageMat& image);

    /**
     * @brief Applies a thinning operation to the image.
     *
     * @param image Image to apply a thinning operation.
     */
    virtual void thinningImage(computerVision::ImageMat& image);

    /**
     * @brief Detects edges the image.
     *
     * @param image Image to detect edges.
     */
    virtual void edgesImage(computerVision::ImageMat& image);

private:
    /** Size of the kernel for filter (must be odd and positive). */
    const unsigned int cFilterKernelSize{9};

    /** Maximum value for thresholding. */
    const double cThresholdMaxValue{255};
    /** Adaptive thresholding algorithm. */
    const computerVision::OpenCvWrapper::AdaptiveThresholdAlgorithm cThresholdMethod{
        computerVision::OpenCvWrapper::AdaptiveThresholdAlgorithm::ADAPTIVE_THRESH_GAUSSIAN};
    /** Threshold operation type. */
    const computerVision::OpenCvWrapper::ThresholdOperations cThresholdOp{
        computerVision::OpenCvWrapper::ThresholdOperations::THRESH_BINARY_INV};
    /** Block size for thresholding. */
    const int cThresholdBlockSize{21};
    /** Constant to subtract from the algorithm for thresholding. */
    const double cThresholdSubConst{4};

    /** Size of the kernel for morphological opening. */
    const unsigned int cMorphOpenKernelSize{3};
    /** Iterations for morphological opening. */
    const unsigned int cMorphOpenIter{1};

    /** Size of the kernel for morphological dilation. */
    const unsigned int cMorphDilateKernelSize{3};
    /** Iterations for morphological dilation. */
    const unsigned int cMorphDilateIter{1};

    /** Threshold1 value for the Canny Edge Detector. */
    const double cCannyEdgeThresh1{50};
    /** Threshold2 value for the Canny Edge Detector. */
    const double cCannyEdgeThresh2{200};
    /** Aperture size for the Sobel operator, for the Canny Edge Detector. */
    const int cCannyEdgeApertureSize{3};

    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Flag to save images obtained during the processing in the working directory. */
    bool mSaveImages{false};
};

} // namespace imageProcessing
} // namespace circuitSegmentation
