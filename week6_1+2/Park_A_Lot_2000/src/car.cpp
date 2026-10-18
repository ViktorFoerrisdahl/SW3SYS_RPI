#include <iostream>
#include <unistd.h>
#include "car.h"

void Car::enter_wait_exit()
{
    entryGate.requestEntry();
    entryGate.wait_until_open_entry(id_);

    sleep(waitTime_);
    std::cout << "Bil nummer " << id_ << " har ventet i " << waitTime_ << " sekunder." << std::endl;

    exitGate.requestEntry();
    exitGate.wait_until_open_exit(id_);
}
int Car::get_CarID()
{
    return id_;
};
