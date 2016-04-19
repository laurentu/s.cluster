#include "Types.h"
#include "Configuration.h"
#include "Status.h"

#include <iostream>
#include <thread>
#include <signal.h>


namespace stupid {
/*
 * Runs a script or exe that should return 0 if server
 * and services are healthy
 * This thread is started by main process and sleeps between
 * each check (self.health.interval).
 */


void self_health_check(const Configuration &conf, Status &status)
{
    // Loop until end of process is signaled (kill)
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

} // end of namespace stupid
