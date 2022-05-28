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
    MOCK_METHOD(void, setLogMode, (const bool& silent), (override));
    /** Mocks method getLogMode. */
    MOCK_METHOD(bool, getLogMode, (), (const, override));
    /** Mocks method showImage. */
    MOCK_METHOD(void, showImage, (const std::string& windowName, ImageMat& image, int delay), (override));
    /** Mocks method writeImage. */
    MOCK_METHOD(bool, writeImage, (const std::string& fileName, ImageMat& image), (override));
    /** Mocks method readImage. */
    MOCK_METHOD(ImageMat, readImage, (const std::string& fileName), (override));
    /** Mocks method cloneImage. */
    MOCK_METHOD(ImageMat, cloneImage, (ImageMat & image), (override));
    /** Mocks method isImageEmpty. */
    MOCK_METHOD(bool, isImageEmpty, (ImageMat & image), (override));
    /** Mocks method resizeImage. */
    MOCK_METHOD(void, resizeImage, (ImageMat & srcImg, ImageMat& dstImg, const double& scale), (override));
    /** Mocks method getImageWidth. */
    MOCK_METHOD(int, getImageWidth, (ImageMat & image), (const, override));
    /** Mocks method getImageHeight. */
    MOCK_METHOD(int, getImageHeight, (ImageMat & image), (const, override));
    /** Mocks method convertImageToGray. */
    MOCK_METHOD(void, convertImageToGray, (ImageMat & srcImg, ImageMat& dstImg), (override));
    /** Mocks method gaussianBlurImage. */
    MOCK_METHOD(void,
                gaussianBlurImage,
                (ImageMat & srcImg, ImageMat& dstImg, const unsigned int& kernelSize),
                (override));
    /** Mocks method adaptiveThresholdImage. */
    MOCK_METHOD(void,
                adaptiveThresholdImage,
                (ImageMat & srcImg,
                 ImageMat& dstImg,
                 const double& maxValue,
                 const AdaptiveThresholdAlgorithm& adaptiveMethod,
                 const ThresholdOperations& thresholdOp,
                 const int& blockSize,
                 const double& subConst),
                (override));
    /** Mocks method cannyEdgeImage. */
    MOCK_METHOD(void,
                cannyEdgeImage,
                (ImageMat & srcImg,
                 ImageMat& dstImg,
                 const double& threshold1,
                 const double& threshold2,
                 const int& apertureSize),
                (override));
};

} // namespace computerVision
} // namespace circuitSegmentation
