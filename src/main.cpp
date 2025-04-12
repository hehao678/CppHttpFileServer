
#include "../include/log.h"
#include "../include/http_server.h"

int main()
{
    Logger &logger = Logger::getInstance();
    logger.setLogLevel(LogLevel::DEBUG);
    logger.setLogFile("./logs/server.log");

    // 日志测试记录
    // LOG_DEBUG("This is a debug log.");
    // LOG_INFO("This is an info log.");
    // LOG_WARNING("This is a warning log.");
    // LOG_ERROR("This is an error log.");

    // http server 测试
    HttpServer server("127.0.0.1", 8080);
    server.start();
    return 0;
}
