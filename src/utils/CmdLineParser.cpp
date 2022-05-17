/**
 * @file
 */

#include "CmdLineParser.h"
#include <algorithm>
#include <iostream>

using namespace circuitSegmentation::utils;

CmdLineParser::CmdLineParser()
    : mCmdLineArgs{}
{
}

void CmdLineParser::parse(const int argc, char const* argv[])
{
    for (int i{0}; i < argc; ++i) {
        mCmdLineArgs.push_back(std::string(argv[i]));
    }
}

std::vector<std::string> CmdLineParser::getArgs() const
{
    return mCmdLineArgs;
}

bool CmdLineParser::hasOption(const std::string& option) const
{
    auto it = std::find(mCmdLineArgs.begin(), mCmdLineArgs.end(), option);

    return (it != mCmdLineArgs.end()) ? true : false;
}

std::string CmdLineParser::getOption(const std::string& option) const
{
    auto it = std::find(mCmdLineArgs.begin(), mCmdLineArgs.end(), option);

    if (it != mCmdLineArgs.end() && ++it != mCmdLineArgs.end()) {
        return *it;
    }

    return "";
}

void CmdLineParser::setAppName(const std::string& name)
{
    mAppName = std::move(name);
}

std::string CmdLineParser::getAppName() const
{
    return mAppName;
}

void CmdLineParser::setAppVersion(const std::string& version)
{
    mAppVersion = std::move(version);
}

std::string CmdLineParser::getAppVersion() const
{
    return mAppVersion;
}

void CmdLineParser::setAppDescription(const std::string& description)
{
    mAppDescription = std::move(description);
}

std::string CmdLineParser::getAppDescription() const
{
    return mAppDescription;
}

void CmdLineParser::showVersion() const
{
    std::cout << mAppName << " " << mAppVersion << std::endl;
    std::cout << mAppDescription << std::endl;
}

void CmdLineParser::setAppUsageInfo(const std::string& exeName,
                                    const std::string& optionsUsageInfo,
                                    const std::map<std::string, std::string> options)
{
    mExeAppName = std::move(exeName);
    mOptionsUsageInfo = std::move(optionsUsageInfo);
    mOptions = options;
}

std::string CmdLineParser::getExeAppName() const
{
    return mExeAppName;
}

std::string CmdLineParser::getOptionsUsageInfo() const
{
    return mOptionsUsageInfo;
}

std::map<std::string, std::string> CmdLineParser::getOptions() const
{
    return mOptions;
}

void CmdLineParser::showHelp() const
{
    std::cout << "Usage: " << mExeAppName << " " << mOptionsUsageInfo << std::endl;

    for (const auto& it : mOptions) {
        std::cout << "\t" << it.first << "\t\t" << it.second << std::endl;
    }
}
