/**
 * @file
 */

#include "ImageSegmentation.h"

namespace circuitSegmentation {
namespace imageProcessing {

using namespace circuitSegmentation::computerVision;

ImageSegmentation::ImageSegmentation(std::shared_ptr<computerVision::OpenCvWrapper> openCvWrapper,
                                     std::shared_ptr<logging::Logger> logger,
                                     bool saveImages)
    : mOpenCvWrapper{std::move(openCvWrapper)}
    , mLogger{std::move(logger)}
    , mSaveImages{std::move(saveImages)}
{
}

bool ImageSegmentation::segmentImage(computerVision::ImageMat imageInitial, computerVision::ImageMat imagePreprocessed)
{
    mLogger->logInfo("Starting image segmentation");

    mImageInitial = std::move(imageInitial);
    mImagePreprocessed = std::move(imagePreprocessed);

    /*
    Steps:
    - Find components
        - Morphological closing is applied which results in dilating components
        - Morphological opening removes the rest of the circuit leaving only the dilated components
        - Save bounding boxes of the components identified
    - Find terminal points of the components
        - Generate a binary image with only bounding boxes
        - Identify terminal points as intersection points between the circuit and bounding boxes generated (binary
    images)
        - See truth table of the article
    - Detection of components
        - Discard bounding boxes of components that do not have terminals
        - Generate images (ROI) with the components
    - Separation of components and circuits
        - Bounding boxes as black pixels, so the components are removed
    - Identification of horizontal and vertical lines
        - Hough transform is applied to identify horizontal and vertical lines
        - See slopes considered for horizontal and vertical lines in the article
    - Detection of nodes
        - Nodes are intersection points of horizontal and vertical lines
        - Dilate image
        - See algorithm used for intersection in the article
    - Mapping nodes to the terminals
        - Each terminal is connected to a node, and hence the connections can be made by choosing the minimum distance
    between a particular terminal and all the nodes in the circuit
        - See pseudo code in the article
    */

    // Find components
    if (!findComponents()) {
        return false;
    }

    // Find terminal points of the components
    if (!findTerminalPoints()) {
        return false;
    }

    return true;
}

void ImageSegmentation::setSaveImages(const bool& saveImages)
{
    mSaveImages = saveImages;
}

bool ImageSegmentation::getSaveImages() const
{
    return mSaveImages;
}

bool ImageSegmentation::findComponents()
{
    mLogger->logInfo("Finding components");

    // Morphological closing
    auto kernelMorph{
        mOpenCvWrapper->getStructuringElement(OpenCvWrapper::MorphShapes::MORPH_RECT, cMorphCloseKernelSize)};
    mOpenCvWrapper->morphologyEx(
        mImagePreprocessed, mImageSegmented, OpenCvWrapper::MorphTypes::MORPH_CLOSE, kernelMorph, cMorphCloseIter);

    mLogger->logInfo("Morphological closing applied to the image, to find components");

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_segment_morph_close.png", mImageSegmented);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological closing to find components", mImageSegmented, 0);
    }

    // Morphological opening
    kernelMorph = mOpenCvWrapper->getStructuringElement(OpenCvWrapper::MorphShapes::MORPH_RECT, cMorphOpenKernelSize);
    mOpenCvWrapper->morphologyEx(
        mImageSegmented, mImageSegmented, OpenCvWrapper::MorphTypes::MORPH_OPEN, kernelMorph, cMorphOpenIter);

    mLogger->logInfo("Morphological opening applied to the image, to find components");

    // Save image
    if (mSaveImages) {
        mOpenCvWrapper->writeImage("image_segment_morph_open.png", mImageSegmented);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Morphological opening to find components", mImageSegmented, 0);
    }

    Contours contours{};
    ContoursHierarchy hierarchy{};

    // Find contours that represent the components (blobs) in the image
    mOpenCvWrapper->findContours(
        mImageSegmented, contours, hierarchy, cBoundingBoxFindContourMode, cBoundingBoxFindContourMethod);

    mLogger->logDebug("Contours found in the image, to find components: " + std::to_string(contours.size()));

    const auto imgWidth{mOpenCvWrapper->getImageWidth(mImageInitial)};
    const auto imgHeight{mOpenCvWrapper->getImageHeight(mImageInitial)};
    mRectComponentsFound.clear();

    // Bounding boxes for all contours
    for (const auto contour : contours) {
        // Check contour area
        if (mOpenCvWrapper->contourArea(contour) < cBoundingBoxMinArea) {
            continue;
        }

        // Bounding rectangle
        auto rect{mOpenCvWrapper->boundingRect(contour)};

        // Increase dimensions because bounding boxes may not completely enclose the component

        auto x{mOpenCvWrapper->getRectCoordX(rect) - cBoundingBoxWidthIncrease / 2};
        x = x < 0 ? 0 : x; // Cannot be negative
        auto y{mOpenCvWrapper->getRectCoordY(rect) - cBoundingBoxHeightIncrease / 2};
        y = y < 0 ? 0 : y; // Cannot be negative

        auto width{mOpenCvWrapper->getRectWidth(rect) + cBoundingBoxWidthIncrease};
        if ((x + width) > imgWidth) {
            width = imgWidth - x;
        }
        auto height{mOpenCvWrapper->getRectHeight(rect) + cBoundingBoxHeightIncrease};
        if ((y + height) > imgHeight) {
            height = imgHeight - y;
        }

        const auto rectComponent{mOpenCvWrapper->createRect(x, y, width, height)};

        mRectComponentsFound.push_back(rectComponent);
    }

    mLogger->logInfo("Components found in the image: " + std::to_string(mRectComponentsFound.size()));

    // Check if there are components found
    if (mRectComponentsFound.empty()) {
        return false;
    }

    mLogger->logInfo("Saved bounding boxes with found components");

    // Save image
    if (mSaveImages) {
        mImageSegmented = mOpenCvWrapper->cloneImage(mImageInitial);
        for (const auto rect : mRectComponentsFound) {
            mOpenCvWrapper->rectangle(
                mImageSegmented, rect, cBoundingBoxColor, cBoundingBoxThickness, OpenCvWrapper::LineTypes::LINE_8);
        }

        mOpenCvWrapper->writeImage("image_segment_find_components.png", mImageSegmented);
        // TODO: Remove or comment.
        mOpenCvWrapper->showImage("Finding components", mImageSegmented, 0);
    }

    return true;
}

bool ImageSegmentation::findTerminalPoints()
{
    mLogger->logInfo("Finding terminal points of components");

    // See https://stackoverflow.com/questions/38457744/opencv-c-collision-for-retangle-and-line

    return true;
}

// // TODO: Remove.
// #include <iostream>
// #include <opencv2/core.hpp>
// #include <opencv2/imgproc.hpp>
// #include <vector>

// void ImageSegmentation::debug(computerVision::ImageMat& image)
// {
//     cv::Mat testImage{image.clone()};
//     cv::Mat kernel{cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1))};

//     int op{0};
//     int value{0};
//     int loop{1};
//     int restart{1};
//     cv::MorphTypes morphOp{};
//     std::vector<std::vector<cv::Point>> testContours;
//     std::vector<cv::Vec4i> contHierarchy;

//     while (loop) {
//         std::cout << "--------" << std::endl;
//         std::cout << "Operations" << std::endl;
//         std::cout << "0: Erosion" << std::endl;
//         std::cout << "1: Dilation" << std::endl;
//         std::cout << "2: Opening" << std::endl;
//         std::cout << "3: Closing" << std::endl;
//         std::cout << "4: Find and draw contours" << std::endl;
//         std::cout << "Select operation: ";
//         std::cin >> op;
//         std::cout << std::endl;

//         if (op != 4) {
//             std::cout << "Kernel size: ";
//             std::cin >> value;
//             std::cout << std::endl;
//             kernel = cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, cv::Size(value, value), cv::Point(-1,
//             -1));
//         }

//         switch (op) {
//         case 0:
//             morphOp = cv::MorphTypes::MORPH_ERODE;
//             break;
//         case 1:
//             morphOp = cv::MorphTypes::MORPH_DILATE;
//             break;
//         case 2:
//             morphOp = cv::MorphTypes::MORPH_OPEN;
//             break;
//         case 3:
//             morphOp = cv::MorphTypes::MORPH_CLOSE;
//             break;
//         default:
//             morphOp = cv::MorphTypes::MORPH_ERODE;
//         }

//         if (op != 4) {
//             std::cout << "Iterations: ";
//             std::cin >> value;
//             std::cout << std::endl;
//         } else {
//             std::cout << "Contour retrieval mode (0 = RETR_EXTERNAL, 1 = RETR_LIST, 2 = RETR_CCOMP, 3 = RETR_TREE, 4
//             = "
//                          "RETR_FLOODFILL): ";
//             std::cin >> value;
//             std::cout << std::endl;
//         }

//         std::cout << "Restart with the initial image: ";
//         std::cin >> restart;
//         std::cout << std::endl;

//         if (restart) {
//             testImage = image.clone();
//             if (op != 4) {
//                 cv::morphologyEx(image, testImage, morphOp, kernel, cv::Point(-1, -1), value);
//             } else {
//                 cv::findContours(
//                     image, testContours, contHierarchy, static_cast<cv::RetrievalModes>(value),
//                     cv::CHAIN_APPROX_SIMPLE);
//                 std::cout << "Number of contours found: " << testContours.size() << std::endl;
//                 int idx = 0;
//                 for (; idx >= 0; idx = contHierarchy[idx][0]) {
//                     cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);
//                     cv::drawContours(testImage, testContours, idx, color, cv::FILLED, cv::LINE_8, contHierarchy);
//                 }
//                 // cv::drawContours(
//                 //     testImage, testContours, -1, cv::Scalar(255, 255, 255), cv::FILLED, cv::LINE_8,
//                 contHierarchy);
//             }
//         } else {
//             if (op != 4) {
//                 cv::morphologyEx(testImage, testImage, morphOp, kernel, cv::Point(-1, -1), value);
//             } else {
//                 cv::findContours(testImage,
//                                  testContours,
//                                  contHierarchy,
//                                  static_cast<cv::RetrievalModes>(value),
//                                  cv::CHAIN_APPROX_SIMPLE);
//                 std::cout << "Number of contours found: " << testContours.size() << std::endl;
//                 int idx = 0;
//                 for (; idx >= 0; idx = contHierarchy[idx][0]) {
//                     cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);
//                     cv::drawContours(testImage, testContours, idx, color, cv::FILLED, cv::LINE_8, contHierarchy);
//                 }
//                 // cv::drawContours(
//                 //     testImage, testContours, -1, cv::Scalar(255, 255, 255), cv::FILLED, cv::LINE_8,
//                 contHierarchy);
//             }
//         }

//         mOpenCvWrapper->writeImage("image_segment_test.png", testImage);
//         mOpenCvWrapper->showImage("Test", testImage, 0);

//         std::cout << "Continue looping (0 to stop): ";
//         std::cin >> loop;
//         std::cout << std::endl;
//     }
// }

} // namespace imageProcessing
} // namespace circuitSegmentation
