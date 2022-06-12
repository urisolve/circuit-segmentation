/**
 * @file
 */

#include "Application.h"
#include "CommandLineParser.h"
#include "imageProcessing/ImageProcManager.h"
#include "logging/Logger.h"
#include <iostream>
#include <memory>

namespace circuitSegmentation {
namespace application {

int Application::exec(int& argc, char const* argv[])
{
    std::shared_ptr<logging::Logger> logger{std::make_shared<logging::Logger>(std::cout)};
    std::unique_ptr<CommandLineParser> parser{std::make_unique<CommandLineParser>()};

    parser->parse(argc, argv);

    // Check if help message was shown
    if (parser->hasHelp()) {
        return 0;
    }

    // Check if version was shown
    if (parser->hasVersion()) {
        return 0;
    }

    // Verbose logs
    const auto hasVerboseLogs{parser->hasVerboseLog()};
    if (hasVerboseLogs) {
        logger->setLogLevel(logging::Logger::LogLevel::VERBOSE);
    } else {
        logger->setLogLevel(logging::Logger::LogLevel::NONE);
    }

    // Image path
    const auto imagePath{parser->getImagePath()};
    if (imagePath.empty()) {
        return 0;
    }

    // Save images obtained during the processing
    const auto hasSaveImages{parser->hasSaveImages()};

    // Proceed with the application
    logger->logInfo("Starting " + std::string(cAppName) + ": version " + std::string(cAppVersion));

    // Image processing manager
    auto imageProcManager{imageProcessing::ImageProcManager::create(logger, hasVerboseLogs, hasSaveImages)};

    // Initialize processing
    imageProcManager.processImage(imagePath);

    logger->logInfo("Ending " + std::string(cAppName) + ": version " + std::string(cAppVersion));

    return 0;
}

} // namespace application
} // namespace circuitSegmentation
