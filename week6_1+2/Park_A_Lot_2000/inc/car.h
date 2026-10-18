#ifndef CAR_H
#define CAR_H

#include <mutex>
#include "gate.h"

class Car
{
private:
    Gate &entryGate;
    Gate &exitGate;
    int id_;
    int waitTime_ = 0;

public:
    Car(Gate &entry, Gate &exit, int id, int waitTime)
        : entryGate(entry), exitGate(exit), id_(id), waitTime_(waitTime) {}

    void enter_wait_exit();
    int get_CarID();
};

#endif