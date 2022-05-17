/**
 * @file
 */

#include "CommandLineParser.h"
#include "Application.h"
#include <iostream>

using namespace circuitSegmentation::application;

void CommandLineParser::parse(const int argc, char const* argv[])
{
    // Set parser information
    mParser.setAppName(Application::cAppName);
    mParser.setAppVersion(Application::cAppVersion);
    const std::string appDescription = std::string(Application::cAppDescription) + "\n" + Application::cAppDate;
    mParser.setAppDescription(appDescription);

    const std::map<std::string, std::string> options{
        {"-h, --help", "show help message"},
        {"-v, --version", "show version"},
        {"-V, --verbose", "enable verbose logs"},
        {"-i, --image", "image file path with the circuit"},
        {"-s, --save-proc", "save images obtained during the processing in the working directory"},
    };
    mParser.setAppUsageInfo(Application::cAppExeName, "-i <image_path> [OPTIONS]", options);

    // Parse
    mParser.parse(argc, argv);
}

bool CommandLineParser::hasHelp() const
{
    // Help
    if (mParser.hasOption("-h") || mParser.hasOption("--help")) {
        mParser.showHelp();
        return true;
    }

    return false;
}

bool CommandLineParser::hasVersion() const
{
    // Version
    if (mParser.hasOption("-v") || mParser.hasOption("--version")) {
        mParser.showVersion();
        return true;
    }

    return false;
}

bool CommandLineParser::hasVerboseLog() const
{
    // Verbose
    if (mParser.hasOption("-V") || mParser.hasOption("--verbose")) {
        return true;
    }

    return false;
}

std::string CommandLineParser::getImagePath() const
{
    // Option
    auto option = mParser.getOption("-i");
    if (option.empty()) {
        option = mParser.getOption("--image");
        if (option.empty()) {
            std::cout << "Missing image input" << std::endl;
            std::cout << std::endl;
            mParser.showHelp();
            return "";
        }
    }

    return option;
}

bool CommandLineParser::hasSaveImages() const
{
    // Verbose
    if (mParser.hasOption("-s") || mParser.hasOption("--save-proc")) {
        return true;
    }

    return false;
}
