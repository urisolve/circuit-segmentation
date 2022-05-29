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
     * @param saveImages Save images obtained during the processing.
     */
    explicit ImageSegmentation(std::shared_ptr<computerVision::OpenCvWrapper> openCvWrapper,
                               std::shared_ptr<logging::Logger> logger,
                               bool saveImages = false);

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

#ifndef BUILD_TESTS
private:
#endif
    /**
     * @brief Finds the components of the circuit.
     *
     * @return True if there are components found, otherwise false.
     */
    virtual bool findComponents();

    /**
     * @brief Finds the terminal points of circuit components.
     *
     * @return True if there are terminal points found, otherwise false.
     */
    virtual bool findTerminalPoints();

private:
    /** Size of the kernel for morphological closing. */
    const unsigned int cMorphCloseKernelSize{11};
    /** Iterations for morphological closing. */
    const unsigned int cMorphCloseIter{5};

    /** Size of the kernel for morphological opening. */
    const unsigned int cMorphOpenKernelSize{3};
    /** Iterations for morphological opening. */
    const unsigned int cMorphOpenIter{3};

    /** Mode of contour retrieval algorithm for bounding boxes. */
    const computerVision::OpenCvWrapper::RetrievalModes cBoundingBoxFindContourMode{
        computerVision::OpenCvWrapper::RetrievalModes::RETR_EXTERNAL};
    /** Contour approximation algorithm for bounding boxes. */
    const computerVision::OpenCvWrapper::ContourApproximationModes cBoundingBoxFindContourMethod{
        computerVision::OpenCvWrapper::ContourApproximationModes::CHAIN_APPROX_SIMPLE};
    /** Bounding boxes minimum area. */
    const double cBoundingBoxMinArea{300};
    /** Bounding boxes color. */
    const computerVision::Scalar cBoundingBoxColor{0, 255, 0};
    /** Bounding boxes thickness. */
    const int cBoundingBoxThickness{3};
    /** Width to increase the bounding box. */
    const int cBoundingBoxWidthIncrease{20};
    /** Height to increase the bounding box. */
    const int cBoundingBoxHeightIncrease{20};

    /** Rectangles of found components. */
    std::vector<computerVision::Rectangle> mRectComponentsFound;

    /** OpenCV wrapper. */
    std::shared_ptr<computerVision::OpenCvWrapper> mOpenCvWrapper;

    /** Logger. */
    std::shared_ptr<logging::Logger> mLogger;

    /** Flag to save images obtained during the processing in the working directory. */
    bool mSaveImages{false};

    /** Initial image. */
    computerVision::ImageMat mImageInitial{};
    /** Preprocessed image. */
    computerVision::ImageMat mImagePreprocessed{};
    /** Segmented image. */
    computerVision::ImageMat mImageSegmented{};
};

} // namespace imageProcessing
} // namespace circuitSegmentation
