
#include "../include/log.h"

int main()
{
    Logger &logger = Logger::getInstance();
    logger.setLogLevel(LogLevel::DEBUG);
    logger.setLogFile("./logs/server.log");

    LOG_DEBUG("This is a debug log.");
    LOG_INFO("This is an info log.");
    LOG_WARNING("This is a warning log.");
    LOG_ERROR("This is an error log.");

    return 0;
}
