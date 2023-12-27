#include "global.h"

LogLevel gLogLevel = LOG_WARN;

void zigbang_log(LogLevel level, const std::string &message)
{
    if (gLogLevel <= level)
    {
        switch (level)
        {
        case LOG_INFO:
            std::cout << "[INFO] " << message << std::endl;
            break;
        case LOG_WARN:
            std::cout << "[WARNING] " << message << std::endl;
            break;
        case LOG_FATAL:
            std::cout << "[FATAL] " << message << std::endl;
            break;
        default:
            std::cout << "[UNKNOWN] " << message << std::endl;
            break;
        }

        std::cout.flush();
    }
}
