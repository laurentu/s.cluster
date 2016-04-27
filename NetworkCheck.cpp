#include "Types.h"
#include "Configuration.h"
#include "Status.h"
#include "Logger.h"

#include <iostream>
#include <thread>
#include <signal.h>

namespace stupid{

void network_check(const Configuration &conf, Status &status)
{
    Logger log("NetworkChecker");
    while(!status.ShouldExit())
    {
        int                 ret = 0;
        string_value_list   values;
        int                 interval = 0;
        int                 exit_status = 0;
        bool                pinged = false;

        string_value_list ping_hosts;
        ret = conf.GetKeyValues("ping.nodes",ping_hosts);
        if(ret != ErrType::Ok)
        {
            log.LogErr("Error getting ping hosts");
        }

        ret = conf.GetKeyValues("ping.nodes.interval",values);
        if(ret == ErrType::Ok && values.size()>0)
            interval = std::stoi(values[0]);
        else
            interval = 1;

        for(auto const &host :ping_hosts)
        {

            std::string cmd = "ping -c1 -W2 " + host;

            std::cout << "Running " << cmd << std::endl;
            ret = system(cmd.c_str());
            exit_status = WEXITSTATUS(ret);
            if (WIFSIGNALED(ret) && (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT))
            {
                     break;
            }
            if(exit_status == 0)
            {
                pinged = true;
                break;
            }
        }
        if(pinged)
            status.SetNetworkStatus(0);
        else
            status.SetNetworkStatus(1);

        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
}

} // end of namespace stupid
