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
	size_t pos = 0;
	while(pos != std::string::npos){
		pos = str.find_first_of(sep, pos);
		if(pos != std::string::npos)
			v.push_back(str.subst());
	}
	return v;
}

} /* namespace stupid */

