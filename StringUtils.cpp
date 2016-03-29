/*
 * StringUtils.cpp
 *
 *  Created on: 25 mars 2016
 *      Author: lulrich
 */

#include "StringUtils.h"

namespace stupid {

StringUtils::StringUtils() {
	// TODO Auto-generated constructor stub

}

StringUtils::~StringUtils() {
	// TODO Auto-generated destructor stub
}

std::vector<std::string> StringUtils::Explode(const std::string &str, const std::string &sep)
{
	std::vector<std::string> v;
    std::string tmp;

    for( auto c: str)
    {
        if( sep.find(c) != sep.npos)
        {
            if(tmp.length()>0) v.push_back(tmp);
            tmp.clear();
        }
        else
        {
            tmp += c;
        }
    }
    if(tmp.length()>0) v.push_back(tmp);
    return v;
}

std::string StringUtils::Implode(const std::vector<std::string> array, const char sep)
{
    std::string ret;
    for( auto const &s: array)
    {
        if(ret.length()>0) ret += sep;
        ret += s;

    }
    return ret;
}
} /* namespace stupid */

