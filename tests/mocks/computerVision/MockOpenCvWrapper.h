/**
 * @file
 */

#pragma once

#include "computerVision/OpenCvWrapper.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace computerVision {

/**
 * @brief Mock of the OpenCvWrapper class.
 */
class MockOpenCvWrapper : public OpenCvWrapper
{
public:
    /** Mocks method setLogMode. */
    MOCK_METHOD(void, setLogMode, (const bool&), (override));
    /** Mocks method getLogMode. */
    MOCK_METHOD(bool, getLogMode, (), (const, override));
    /** Mocks method showImage. */
    MOCK_METHOD(void, showImage, (const std::string&, ImageMat&, int), (override));
    /** Mocks method writeImage. */
    MOCK_METHOD(bool, writeImage, (const std::string&, ImageMat&), (override));
    /** Mocks method readImage. */
    MOCK_METHOD(ImageMat, readImage, (const std::string&), (override));
    /** Mocks method cloneImage. */
    MOCK_METHOD(ImageMat, cloneImage, (ImageMat&), (override));
    /** Mocks method isImageEmpty. */
    MOCK_METHOD(bool, isImageEmpty, (ImageMat&), (override));
    /** Mocks method resizeImage. */
    MOCK_METHOD(void, resizeImage, (ImageMat&, ImageMat&, const double&), (override));
    /** Mocks method getImageWidth. */
    MOCK_METHOD(int, getImageWidth, (ImageMat&), (const, override));
    /** Mocks method getImageHeight. */
    MOCK_METHOD(int, getImageHeight, (ImageMat&), (const, override));
    /** Mocks method convertImageToGray. */
    MOCK_METHOD(void, convertImageToGray, (ImageMat&, ImageMat&), (override));
    /** Mocks method gaussianBlurImage. */
    MOCK_METHOD(void, gaussianBlurImage, (ImageMat&, ImageMat&, const unsigned int&), (override));
    /** Mocks method adaptiveThresholdImage. */
    MOCK_METHOD(void,
                adaptiveThresholdImage,
                (ImageMat&,
                 ImageMat&,
                 const double&,
                 const AdaptiveThresholdAlgorithm&,
                 const ThresholdOperations&,
                 const int&,
                 const double&),
                (override));
    /** Mocks method cannyEdgeImage. */
    MOCK_METHOD(void, cannyEdgeImage, (ImageMat&, ImageMat&, const double&, const double&, const int&), (override));
    /** Mocks method getStructuringElement. */
    MOCK_METHOD(ImageMat, getStructuringElement, (const MorphShapes&, const unsigned int&), (override));
    /** Mocks method morphologyEx. */
    MOCK_METHOD(void,
                morphologyEx,
                (ImageMat&, ImageMat&, const MorphTypes&, const ImageMat&, const unsigned int&),
                (override));
    /** Mocks method findContours. */
    MOCK_METHOD(void,
                findContours,
                (ImageMat&, Contours&, ContoursHierarchy&, const RetrievalModes&, const ContourApproximationModes&),
                (override));
    /** Mocks method drawContours. */
    MOCK_METHOD(
        void,
        drawContours,
        (ImageMat&, const Contours&, const int&, const Scalar&, const int&, const LineTypes&, const ContoursHierarchy&),
        (override));
    /** Mocks method contourArea. */
    MOCK_METHOD(double, contourArea, (InputOutputArray&), (override));
    /** Mocks method arcLength. */
    MOCK_METHOD(double, arcLength, (InputOutputArray&, const bool&), (override));
    /** Mocks method boundingRect. */
    MOCK_METHOD(Rectangle, boundingRect, (InputOutputArray&), (override));
    /** Mocks method rectangle. */
    MOCK_METHOD(void, rectangle, (ImageMat&, const Rectangle&, const Scalar&, const int&, const LineTypes&), (override));
    /** Mocks method contains. */
    MOCK_METHOD(bool, contains, (const Rectangle&, const Point&), (override));
    /** Mocks method rectangleArea. */
    MOCK_METHOD(int, rectangleArea, (const Rectangle&), (override));
    /** Mocks method thinning. */
    MOCK_METHOD(void, thinning, (ImageMat&, ImageMat&, const ThinningAlgorithms&), (override));
    /** Mocks method bitwiseAnd. */
    MOCK_METHOD(void, bitwiseAnd, (InputOutputArray&, InputOutputArray&, InputOutputArray&), (override));
};

} // namespace computerVision
} // namespace circuitSegmentation
