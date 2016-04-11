#ifndef STATUS_H
#define STATUS_H

#include <mutex>
#include <string>
#include <map>

namespace stupid {


class Status
{
public:
    Status();
    void SetNetworkStatus(int s);
    void SetSelfHealth(int s);
    void Exit(void);
    int ShouldExit(void);
private:
    std::mutex lock;
    int network_status;
    int self_health;
    std::map<std::string,int> partner_health;
    int exit;
};

}
#endif // STATUS_H
