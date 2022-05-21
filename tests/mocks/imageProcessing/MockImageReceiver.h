/**
 * @file
 */

#pragma once

#include "imageProcessing/ImageReceiver.h"
#include <gmock/gmock.h>

namespace circuitSegmentation {
namespace imageProcessing {

/**
 * @brief Mock of the ImageReceiver class.
 */
class MockImageReceiver : public ImageReceiver
{
public:
    /** Mocks method "virtual bool receiveImage()". */
    MOCK_METHOD(bool, receiveImage, (), (override));
    /** Mocks method "virtual cv::Mat getImageReceived() const". */
    MOCK_METHOD(cv::Mat, getImageReceived, (), (const, override));
    /** Mocks method "virtual void setImageFilePath(const std::string& filePath)". */
    MOCK_METHOD(void, setImageFilePath, (const std::string& filePath), (override));
    /** Mocks method "virtual std::string getImageFilePath() const". */
    MOCK_METHOD(std::string, getImageFilePath, (), (const, override));
};

} // namespace imageProcessing
} // namespace circuitSegmentation
