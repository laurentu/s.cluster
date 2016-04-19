#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <syslog.h>
namespace stupid {

class Logger
{
public:
    Logger(const std::string &thread_name, bool debug = false);
    void LogErr(const std::string &msg);
    void LogInfo(const std::string &msg);
private:
    std::string thread;
    bool debug;
};

} // end of namespace stupid
#endif // LOGGER_H
