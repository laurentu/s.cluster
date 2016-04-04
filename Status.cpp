#include "Status.h"
namespace stupid {


Status::Status()
{
}

void Status::SetNetworkStatus(int s)
{
    this->lock.lock();
    this->network_status = s;
    this->lock.unlock();
}
void Status::SetSelfHealth(int s)
{
    this->lock.lock();
    this->self_health = s;
    this->lock.unlock();
}
}
