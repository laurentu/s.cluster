#include "logger.h"

namespace stupid {


Logger::Logger(const std::string &thread_name, bool debug)
{
    this->debug = debug;
    this->thread = thread_name;
}

} // end of namespace stupid
