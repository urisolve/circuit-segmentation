/**
 * @file
 */

#pragma once

#include <opencv2/core.hpp>
#include <string>

namespace circuitSegmentation {
namespace imageProcessing {

/**
 * @brief Image processing utils.
 */
class ImageProcUtils
{
public:
    /**
     * @brief Constructor of a new object.
     */
    ImageProcUtils() = default;

    /**
     * @brief Destructor of the object.
     */
    virtual ~ImageProcUtils() = default;

    /**
     * @brief Shows the image in a new window.
     *
     * @param windowName Window name.
     * @param image Image.
     * @param delay Delay to wait for a pressed key, in milliseconds. The method waits for a key event infinitely (when
     * delay <= 0), or for the specified delay when it is positive.
     */
    virtual void showImage(const std::string& windowName, cv::Mat& image, int delay);

    /**
     * @brief Writes the image to the file.
     *
     * @param fileName File name.
     * @param image Image.
     *
     * @return Returns true if the operation occurred successfully, otherwise false.
     */
    virtual bool writeImage(const std::string& fileName, cv::Mat& image);
};

} // namespace imageProcessing
} // namespace circuitSegmentation
