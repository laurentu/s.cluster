#include "Logger.h"
#include <iostream>
#include <syslog.h>

namespace stupid {


Logger::Logger(const std::string &thread_name, bool debug)
{
    this->debug = debug;
    this->thread = thread_name;
}

void Logger::LogErr(const std::string &msg)
{
    Log(LVL_ERR,msg);
}
void Logger::LogInfo(const std::string &msg)
{
    Log(LVL_WARN,msg);
}
void Logger::LogDebug(const std::string &msg)
{
    Log(LVL_DEBUG,msg);
}

void Logger::Log(LogLevel lvl, const std::string &msg)
{
    if(debug)
    {
        switch(lvl)
        {
        case LVL_WARN:
        case LVL_ERR:
            std::cerr << msg << std::endl;
            break;
        default:
            std::cout << msg << std::endl;
            break;
        }
    }
    else
    {
        if(lvl == LVL_DEBUG)
            return;
        if(lvl == LVL_WARN)
            syslog(LOG_USER|LOG_WARNING, msg.c_str());
        if(lvl == LVL_ERR)
            syslog(LOG_USER|LOG_ERR, msg.c_str());
    }
}
} // end of namespace stupid
