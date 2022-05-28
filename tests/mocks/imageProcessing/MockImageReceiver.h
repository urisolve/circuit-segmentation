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
    /** Mocks method receiveImage. */
    MOCK_METHOD(bool, receiveImage, (), (override));
    /** Mocks method getImageReceived. */
    MOCK_METHOD(computerVision::ImageMat, getImageReceived, (), (const, override));
    /** Mocks method setImageFilePath. */
    MOCK_METHOD(void, setImageFilePath, (const std::string& filePath), (override));
    /** Mocks method getImageFilePath. */
    MOCK_METHOD(std::string, getImageFilePath, (), (const, override));
};

} // namespace imageProcessing
} // namespace circuitSegmentation
