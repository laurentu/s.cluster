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


#include <oping.h>
namespace stupid {


void network_check(const Configuration &conf, const Status &status);

void network_check(const Configuration &conf, const Status &status)
{
    std::cout << "Ping..." << std::this_thread::get_id() << std::endl;
    conf.Dump();

    pingobj_t   *pinger = nullptr;
    int         ret;
    pinger = ping_construct();
    if(pinger == nullptr)
    {
        std::cout << "ARGGGGGGGHHHHH" << std::endl;
    }
    ret = ping_host_add(pinger, "192.168.1.10");
    if(ret != 0)
    {
        std::cout << "Could not add host " << "192.168.1.10" << std::endl;
    }
    ret = ping_host_add(pinger, "172.17.2.10");
    if(ret != 0)
    {
        std::cout << "Could not add host " << "supervision-1" << std::endl;
    }

    std::cout << "Sending ping..." << std::endl;
    ret = ping_send(pinger);
    std::cout << "Received " << ret << " as response to pings" << std::endl;
    ping_host_remove(pinger, "192.168.1.10");
    ping_host_remove(pinger, "172.17.2.10");
    ping_destroy(pinger);
}
} // end of namespace stupid

using namespace stupid;

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



