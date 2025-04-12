
#include "log.h"
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>

Logger::Logger() : currentLevel(LogLevel::DEBUG) {}

Logger::~Logger() {
  if (logFile.is_open()) {
    logFile.close();
  }
}

Logger &Logger::getInstance() {
  static Logger instance;
  return instance;
}

void Logger::setLogLevel(LogLevel level) { currentLevel = level; }

void Logger::setLogFile(const std::string &filename) {
  std::filesystem::create_directories("./logs/");
  logFile.open(filename, std::ios::app);
}

void Logger::log(LogLevel level, const std::string &message, const char *file,
                 int line) {
  if (level < currentLevel)
    return;

  std::lock_guard<std::mutex> lock(mtx);

  std::string timeStr = getCurrentTime();
  std::string levelStr = logLevelToString(level);
  std::string logMsg = levelStr + " " + timeStr + " [" + file + ":" +
                       std::to_string(line) + "] " + message;

  std::cout << logMsg << std::endl;
  if (logFile.is_open()) {
    logFile << logMsg << std::endl;
  }
}

std::string Logger::getCurrentTime() {
  std::time_t now = std::time(nullptr);
  std::tm *tm_ptr = std::localtime(&now);
  char buffer[100];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_ptr);
  return std::string(buffer);
}

std::string Logger::logLevelToString(LogLevel level) {
  switch (level) {
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
