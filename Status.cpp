#include "Status.h"
namespace stupid {


Status::Status()
{
    this->fatal_error = -1;
    this->network_status = -1;
    this->self_health = -1;

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
void Status::FatalError()
{
    this->fatal_error = 0;
}
}
