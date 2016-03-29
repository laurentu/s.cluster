/*
 * cluster.cpp
 *
 *  Created on: 25 mars 2016
 *      Author: lulrich
 */

#include <iostream>
#include "StringUtils.h"
#include "Configuration.h"

using namespace stupid;
int main(int argc, char **argv)
{

    Configuration cnf;

    cnf.Read("cluster.conf");
    cnf.Dump();

    auto values = StringUtils::Explode("a,b,sdklfj,slmkdfj,", ", \t");
    std::cout << StringUtils::Implode(values, ',') << std::endl;
}



