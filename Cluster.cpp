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
#include <unistd.h>

#include "Types.h"
#include "Status.h"
#include "StringUtils.h"
#include "Configuration.h"



stupid::Status status;

namespace stupid {


void network_check(const Configuration &conf, Status &status);
void self_health_check(const Configuration &conf, Status &status);
void node_check(const Configuration &conf, Status &status);



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
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    signal(SIGINT,signal_handler);
    signal(SIGQUIT,signal_handler);

    std::thread network_check_t(network_check,std::ref(cnf), std::ref(status));
    std::thread self_health_check_t(self_health_check,std::ref(cnf), std::ref(status));
    std::thread node_check_t(node_check,std::ref(cnf), std::ref(status));

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



