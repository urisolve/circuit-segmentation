/**
 * @file
 */

#pragma once

#include "circuit/Component.h"
#include "circuit/Node.h"
#include "common/RandomNumberGen.h"
#include "computerVision/OpenCvWrapper.h"
#include "logging/Logger.h"
#include <memory>
#include <vector>

namespace circuitSegmentation {
namespace imageProcessing {

/**
 * @brief Image segmentation.
 */
class ImageSegmentation
{
public:
    /** Bounding boxes minimum area. */
    static constexpr double cBoundingBoxMinArea{200};
    /** Nodes minimum length. */
    static constexpr double cNodesMinLength{20};

    /**
     * @brief Constructor.
     *
     * @param openCvWrapper OpenCV wrapper.
     * @param logger Logger.
     * @param saveImages Save images obtained during the processing.
     */
    explicit ImageSegmentation(const std::shared_ptr<computerVision::OpenCvWrapper>& openCvWrapper,
                               const std::shared_ptr<logging::Logger>& logger,
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

#ifndef BUILD_TESTS
private:
#endif
    /**
     * @brief Detects the components of the circuit.
     *
     * @return True if there are components detected, otherwise false.
     */
    virtual bool detectComponents();

    /**
     * @brief Generates bounding boxes for the circuit components.
     *
     * @param blobs Components represented as blobs.
     */
    virtual void boundingBoxComponents(const computerVision::Contours& blobs);

    /**
     * @brief Increases size of bounding box and centers it.
     *
     * @param box Bounding box.
     * @param widthIncr Increment for width.
     * @param heightIncr Increment for height.
     * @param widthMax Maximum width (x coordinate + box width).
     * @param heightMax Maximum height (y coordinate + box height).
     *
     * @return Enlarged size bounding box.
     */
    virtual computerVision::Rectangle increaseBoundingBox(const computerVision::Rectangle& box,
                                                          const int& widthIncr,
                                                          const int& heightIncr,
                                                          const int& widthMax,
                                                          const int& heightMax);

    /**
     * @brief Detects the nodes of the circuit.
     *
     * @return True if there are nodes detected, otherwise false.
     */
    virtual bool detectNodes();

    /**
     * @brief Detects circuit connections.
     */
    virtual void detectCircuitConnections();

    /**
     * @brief Update detected components of the circuit.
     *
     * This method verifies that each detected component is connected to a node. If there are no nodes connected to a
     * component, that component is discarded from the list of detected components.
     *
     * @return True if there are components detected, otherwise false.
     */
    virtual bool updateDetectedComponents();

    /**
     * @brief Gets the detected components.
     *
     * @return Detected components.
     */
    [[nodiscard]] virtual const std::vector<circuit::Component>& getDetectedComponents() const;

#ifdef BUILD_TESTS
public:
    /**
     * @brief Sets the detected components.
     *
     * This method is only useful for testing purposes.
     *
     * @param components Detected components.
     */
    virtual void setDetectedComponents(const std::vector<circuit::Component>& components);

    /**
     * @brief Gets the detected nodes.
     *
     * @return Detected nodes.
     */
    [[nodiscard]] virtual const std::vector<circuit::Node>& getDetectedNodes() const;

    /**
     * @brief Sets the detected nodes.
     *
     * This method is only useful for testing purposes.
     *
     * @param nodes Detected nodes.
     */
    virtual void setDetectedNodes(const std::vector<circuit::Node>& nodes);
#endif

private:
    /** Size of the kernel for morphological closing. */
    const unsigned int cMorphCloseKernelSize{11};
    /** Iterations for morphological closing. */
    const unsigned int cMorphCloseIter{4};

    /** Size of the kernel for morphological opening. */
    const unsigned int cMorphOpenKernelSize{3};
    /** Iterations for morphological opening. */
    const unsigned int cMorphOpenIter{1};

    /** Mode of contour retrieval algorithm for bounding boxes. */
    const computerVision::OpenCvWrapper::RetrievalModes cBoundingBoxFindContourMode{
        computerVision::OpenCvWrapper::RetrievalModes::RETR_EXTERNAL};
    /** Contour approximation algorithm for bounding boxes. */
    const computerVision::OpenCvWrapper::ContourApproximationModes cBoundingBoxFindContourMethod{
        computerVision::OpenCvWrapper::ContourApproximationModes::CHAIN_APPROX_SIMPLE};
    /** Bounding boxes color. */
    const computerVision::Scalar cBoundingBoxColor{0, 255, 0};
    /** Bounding boxes thickness. */
    const int cBoundingBoxThickness{3};
    /** Width to increase the bounding box. */
    const int cBoundingBoxWidthIncrease{20};
    /** Height to increase the bounding box. */
    const int cBoundingBoxHeightIncrease{20};

    /** Mode of contour retrieval algorithm for nodes detection. */
    const computerVision::OpenCvWrapper::RetrievalModes cNodesFindContourMode{
        computerVision::OpenCvWrapper::RetrievalModes::RETR_EXTERNAL};
    /** Contour approximation algorithm for nodes detection. */
    const computerVision::OpenCvWrapper::ContourApproximationModes cNodesFindContourMethod{
        computerVision::OpenCvWrapper::ContourApproximationModes::CHAIN_APPROX_SIMPLE};
    /** Node contour color. */
    const computerVision::Scalar cNodesColor{0, 0, 255};
    /** Node contour thickness. */
    const int cNodesThickness{3};

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
    /** Image used during the segmentation process. */
    computerVision::ImageMat mImageSegmentationProc{};

    /** Components detected. */
    std::vector<circuit::Component> mComponents;

    /** Nodes detected. */
    std::vector<circuit::Node> mNodes;
};

} // namespace imageProcessing
} // namespace circuitSegmentation
