#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace stupid {

class Logger
{
    enum LogLevel {
        LVL_DEBUG,
        LVL_WARN,
        LVL_ERR
    };

public:
    Logger(const std::string &thread_name, bool debug = false);
    void LogErr(const std::string &msg);
    void LogInfo(const std::string &msg);
    void LogDebug(const std::string &msg);
private:
    void Log(LogLevel lvl, const std::string &msg);
    std::string thread;
    bool debug;
};

} // end of namespace stupid
#endif // LOGGER_H
