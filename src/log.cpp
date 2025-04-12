#include "log.h"
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <sstream>

Logger::Logger() : currentLevel(LogLevel::DEBUG) {}

Logger::~Logger()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}

// 单例设计模式，使用静态变量
Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::setLogLevel(LogLevel level) { currentLevel = level; }

void Logger::setLogFile(const std::string &filename)
{
    std::filesystem::create_directories("./logs/");
    logFile.open(filename, std::ios::app);
}

void Logger::log(LogLevel level, const std::string &message, const char *file, int line)
{
    if (level < currentLevel)
    {
        return;
    }

    std::lock_guard<std::mutex> lock(mtx);

    std::string timeStr  = getCurrentTime();
    std::string levelStr = logLevelToString(level);
    std::string logMsg   = timeStr + " " + levelStr + " " + message + " Line:" + std::to_string(line);

    std::cout << logMsg << std::endl;
    if (logFile.is_open())
    {
        logFile << logMsg << std::endl;
    }
}

std::string Logger::getCurrentTime()
{
    auto    now        = std::chrono::system_clock::now();
    auto    now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm         = *std::localtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::logLevelToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return "[DEBUG]";
    case LogLevel::INFO:
        return "[INFO]";
    case LogLevel::WARNING:
        return "[WARNING]";
    case LogLevel::ERROR:
        return "[ERROR]";
    default:
        return "[UNKNOWN]";
    }
}