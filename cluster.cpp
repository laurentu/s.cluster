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
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>

#include "Types.h"
#include "Status.h"
#include "StringUtils.h"
#include "Configuration.h"


#include <oping.h>

stupid::Status status;

namespace stupid {


void network_check(const Configuration &conf, Status &status);
void self_health_check(const Configuration &conf, Status &status);

void self_health_check(const Configuration &conf, Status &status)
{

    while(!status.ShouldExit())
    {
        int                 exit_status = 0;
        string_value_list   values;
        std::string         script;
        int                 ret = 0;
        int                 interval = 0;

        std::cout << "Self health checker online..." << std::endl;

        ret = conf.GetKeyValues("self.health", values);
        if(ret != ErrType::Ok || values.size()<=0)
        {
            status.Exit();
            return;
        }
        script = values[0];

        values.empty();
        ret = conf.GetKeyValues("self.health.interval",values);
        if(ret == ErrType::Ok && values.size()>0)
            interval = std::stoi(values[0]);
        else
            interval = 1;
        std::cout << "Running " << script.c_str() << std::endl;
        ret = system(script.c_str());
        exit_status = WEXITSTATUS(ret);
        if (WIFSIGNALED(ret) && (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT))
        {
                 break;
        }
        std::cout << "Script " << script << " returned " << exit_status << std::endl;
        if(exit_status != 0)
            status.SetSelfHealth(1);
        else
            status.SetSelfHealth(0);

        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
}

void network_check(const Configuration &conf, Status &status)
{
    std::cout << "Ping..." << std::this_thread::get_id() << std::endl;
    conf.Dump();

    while(!status.ShouldExit())
    {
        pingobj_t           *pinger = nullptr;
        int                 ret = 0;
        string_value_list   values;
        int                 interval = 0;

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

        ret = conf.GetKeyValues("ping.nodes.interval",values);
        if(ret == ErrType::Ok && values.size()>0)
            interval = std::stoi(values[0]);
        else
            interval = 1;

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
        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
}

void signal_handler(int signum)
{
    status.Exit();
}

} // end of namespace stupid

using namespace stupid;


int main(int argc, char **argv)
{

    pid_t               pid;
    gid_t               gid;
    uid_t               uid;

    int                 ret;
    string_value_list   values;
    Configuration       cnf;
    struct group        *grp = nullptr;
    struct passwd       *pwd = nullptr;


    cnf.Read("cluster.conf");
    cnf.Dump();

    ret = cnf.GetKeyValues("uid",values);
    if( ret != ErrType::Ok || values.size()==0)
    {
        std::cerr << "No uid configured" << std::endl;
        exit(EXIT_FAILURE);
    }

    pwd = getpwnam(values[0].c_str());
    if( pwd == nullptr)
    {
        std::cerr << "User " << values[0] << " does not exist" << std::endl;
        exit(EXIT_FAILURE);
    }

    uid = pwd->pw_uid;

    ret = cnf.GetKeyValues("gid",values);

    if( ret != ErrType::Ok || values.size()==0)
    {
        std::cerr << "No gid configured" << std::endl;
        exit(EXIT_FAILURE);
    }
    grp = getgrnam(values[0].c_str());
    if( grp == nullptr)
    {
        std::cerr << "Group " << values[0] << " does not exist" << std::endl;
        exit(EXIT_FAILURE);
    }
    gid = grp->gr_gid;

    pid = fork();
    if(pid < 0)
    {
        std::cerr << "Error, could not fork !!!!!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(pid>0)
        exit(EXIT_SUCCESS);


    umask(0);

    ret = setgid(gid);
    if(ret != 0)
    {
        std::cerr << "Could not change gid" << std::endl;
        exit(EXIT_FAILURE);
    }
    ret = setuid(uid);
    if(ret != 0)
    {
        std::cerr << "Could not change uid" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(chdir("/") != 0)
    {
        std::cerr << "Could not chdir to /" << std::endl;
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    /*
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    */

    signal(SIGINT,signal_handler);
    signal(SIGQUIT,signal_handler);
    std::cout << "Starting thread" << std::endl;
    std::thread network_check_t(network_check,std::ref(cnf), std::ref(status));
    std::thread self_health_check_t(self_health_check,std::ref(cnf), std::ref(status));

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if(status.ShouldExit())
            break;
    }
    network_check_t.join();
    self_health_check_t.join();

    std::cout << "This is the end" << std::endl;
    return 0;

}



