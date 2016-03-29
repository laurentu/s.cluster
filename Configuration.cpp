#include <fstream>
#include <iostream>
#include "Types.h"
#include "Configuration.h"
#include "StringUtils.h"

namespace stupid {

Configuration::Configuration()
{
}

int Configuration::Read(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    if(!file.is_open())
    {
        return FileNotFound;
    }

    std::string line;
    while(file.good())
    {
        getline(file,line);
        auto tmp = StringUtils::Explode(line,"= \t");
        if(tmp.size()>1)
        {
            (*this)[tmp[0]]= StringUtils::Explode(tmp[1],", \t");
        }
    }
    file.close();

    return Ok;
}

void Configuration::Dump(void)
{
    for( auto const &v:(*this))
    {
        std::cout << v.first << " = " << StringUtils::Implode(v.second, ',') << std::endl;
    }

}
} // end of namespace stupid
