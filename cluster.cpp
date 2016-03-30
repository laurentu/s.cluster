/*
 * cluster.cpp
 *
 *  Created on: 25 mars 2016
 *      Author: lulrich
 */

#include <iostream>
#include <thread>

#include "Types.h"
#include "Status.h"
#include "StringUtils.h"
#include "Configuration.h"

using namespace stupid;


void network_check(const Configuration &conf, const Status &status)
{
    std::cout << "Ping..." << std::this_thread::get_id() << std::endl;
    conf.Dump();
}

int main(int argc, char **argv)
{

    Configuration cnf;
    Status status;

    cnf.Read("cluster.conf");
    cnf.Dump();

    std::cout << "Starting thread" << std::endl;
    std::thread network_check_t(network_check,std::ref(cnf), std::ref(status));


    network_check_t.join();

    std::cout << "This is the end" << std::endl;
    return 0;

}



