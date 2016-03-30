#ifndef TYPES_H
#define TYPES_H

namespace stupid {

enum ErrType {
    Ok = 0,
    FileNotFound = 1
};

struct Status
{
    int self_network_health = 0;
    int self_health = 0;
};
}
#endif // TYPES_H
