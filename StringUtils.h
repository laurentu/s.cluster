/*
 * StringUtils.h
 *
 *  Created on: 25 mars 2016
 *      Author: lulrich
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <string>
#include <vector>
namespace stupid {


class StringUtils {
public:
	StringUtils();
	virtual ~StringUtils();
	static std::vector<std::string> Explode(const std::string &str, const std::string &sep);
    static std::string Implode(const std::vector<std::string> array, const char sep);
};

} /* namespace stupid */

#endif /* STRINGUTILS_H_ */
