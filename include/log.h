
#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <mutex>
#include <string>

enum class LogLevel
{
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR
};

class Logger
{
  public:
    static Logger &getInstance();

    void setLogLevel(LogLevel level);
    void setLogFile(const std::string &filename);
    void log(LogLevel level, const std::string &message, const char *file, int line);

  private:
    Logger();
    ~Logger();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    std::string getCurrentTime();
    std::string logLevelToString(LogLevel level);

    LogLevel      currentLevel;
    std::ofstream logFile;
    std::mutex    mtx;
};

#define LOG_DEBUG(msg) Logger::getInstance().log(LogLevel::DEBUG, msg, __FILE__, __LINE__)
#define LOG_INFO(msg) Logger::getInstance().log(LogLevel::INFO, msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) Logger::getInstance().log(LogLevel::WARNING, msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) Logger::getInstance().log(LogLevel::ERROR, msg, __FILE__, __LINE__)

#endif // LOG_H
