/**
 * @file
 */

#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace circuitSegmentation {
namespace logging {

Logger::Logger(std::ostream& ostream, LogLevel level)
    : mOstream{ostream}
    , mLogLevel{std::move(level)}
{
}

void Logger::setLogLevel(LogLevel level)
{
    mLogLevel = std::move(level);
}

Logger::LogLevel Logger::getLogLevel() const
{
    return mLogLevel;
}

void Logger::logFatal(const std::string& msg)
{
    if (mLogLevel >= LogLevel::FATAL) {
        log("FATAL", msg);
    }
}

void Logger::logError(const std::string& msg)
{
    if (mLogLevel >= LogLevel::ERROR) {
        log("ERROR", msg);
    }
}

void Logger::logWarning(const std::string& msg)
{
    if (mLogLevel >= LogLevel::WARNING) {
        log("WARNING", msg);
    }
}

void Logger::logInfo(const std::string& msg)
{
    if (mLogLevel >= LogLevel::INFO) {
        log("INFO", msg);
    }
}

void Logger::logDebug(const std::string& msg)
{
    if (mLogLevel >= LogLevel::DEBUG) {
        log("DEBUG", msg);
    }
}

void Logger::logVerbose(const std::string& msg)
{
    if (mLogLevel >= LogLevel::VERBOSE) {
        log("VERBOSE", msg);
    }
}

void Logger::log(const std::string& level, const std::string& msg)
{
    mOstream << getDateTime() << "[" << level << "] " << msg << std::endl;
}

const std::string Logger::getDateTime() const
{
    // Time point representing the current point in time
    auto now = std::chrono::system_clock::now();

    // Time converted to std::time_t
    auto nowConv = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss{};
    ss << "[";
    ss << std::put_time(std::localtime(&nowConv), "%Y-%m-%d %X");
    ss << "]";

    return ss.str();
}

} // namespace logging
} // namespace circuitSegmentation
