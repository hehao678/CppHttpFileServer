#ifdef LOG_H
#define LOG_H

#include <string>
#include<fstream>
#include <mutex>

enum class LogLevel{
    DEBUG = 0;
    INFO ,
    WARNING,
    ERROR
};



#endif //LOG_H