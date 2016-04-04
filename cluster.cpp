/*
 * cluster.cpp
 *
 *  Created on: 25 mars 2016
 *      Author: lulrich
 */

#define _XOPEN_SOURCE

#include <iostream>
#include <thread>
#include <signal.h>

#include "Types.h"
#include "Status.h"
#include "StringUtils.h"
#include "Configuration.h"


#include <oping.h>
namespace stupid {


void network_check(const Configuration &conf, Status &status);
void self_health_check(const Configuration &conf, Status &status);

void self_health_check(const Configuration &conf, Status &status)
{
    std::cout << "Self health checker online..." << std::endl;

    while(1)
    {
        string_value_list values;
        int ret;
        ret = conf.GetKeyValues("self.health", values);
        if(ret != ErrType::Ok || values.size()<=0)
        {
            status.FatalError();
            return;
        }
        ret = system(values[0].c_str());
        int exit_status = WEXITSTATUS(ret);
        if (WIFSIGNALED(ret) && (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT))
                 break;
        std::cout << "Script returned " << exit_status << std::endl;
        if(exit_status != 0)
            status.SetSelfHealth(1);
        else
            status.SetSelfHealth(0);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void network_check(const Configuration &conf, Status &status)
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

    string_value_list ping_hosts;
    ret = conf.GetKeyValues("ping.nodes",ping_hosts);
    if(ret != ErrType::Ok)
    {
        std::cout << "Error getting ping hosts" << std::endl;
    }
    for(auto const &host :ping_hosts)
    {
        ret = ping_host_add(pinger, host.c_str());
        if(ret != 0)
            std::cout << "Could not add host " << host << std::endl;
        else
            std::cout << "Host " << host << " added" << std::endl;
    }
    std::cout << "Sending ping..." << std::endl;
    ret = ping_send(pinger);
    if(ret>0)
        status.SetNetworkStatus(0);
    else
        status.SetNetworkStatus(1);

    std::cout << "Received " << ret << " as response to pings" << std::endl;
    for(auto const &host :ping_hosts)
        ret = ping_host_remove(pinger, host.c_str());

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
    std::thread self_health_check_t(self_health_check,std::ref(cnf), std::ref(status));

    network_check_t.join();
    self_health_check_t.join();

    std::cout << "This is the end" << std::endl;
    return 0;

}



