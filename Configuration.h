#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <map>
#include <vector>

namespace stupid {

typedef std::vector<std::string>    string_value_list;

class Configuration : public std::map<std::string, string_value_list>
{
public:
    Configuration();
    int Read(const std::string &filename);
    void Dump(void) const;
    string_value_list GetKeyValue(const std::string &key);
};

}
#endif // CONFIGURATION_H
