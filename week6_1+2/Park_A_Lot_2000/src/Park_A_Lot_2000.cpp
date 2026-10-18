#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>
#include "car.h"
#include "gate.h"

int main()
{
    //Danner gate klasser, som tager enum TYPE_OF_GATE som parametre i deres konstruktør
    Gate gate_entry{ENTRY};
    Gate gate_exit{EXIT};

    //Danner car klasser med Gate klasser, ID og ventetid i parkeringspladsen som parametre
    Car car1{gate_entry, gate_exit, 1, 5};
    Car car2{gate_entry, gate_exit, 2, 11};
    Car car3{gate_entry, gate_exit, 3, 12};
    Car car4{gate_entry, gate_exit, 4, 13};
    Car car5{gate_entry, gate_exit, 5, 15};

    //Starter to threads til vores gates
    std::thread g1([&]{gate_entry.active();});
    std::thread g2([&]{gate_exit.active();});

    sleep(1); //Sætter sleep mellem oprettelse af gates og biler for pænere terminal

    //Starter 5 threads til vores biler
    std::thread c1([&]{car1.enter_wait_exit();});
    std::thread c2([&]{car2.enter_wait_exit();});
    std::thread c3([&]{car3.enter_wait_exit();});
    std::thread c4([&]{car4.enter_wait_exit();});
    std::thread c5([&]{car5.enter_wait_exit();});

    //joiner vores threads
    g1.join();
    c1.join();
    c2.join();
    c3.join();
    c4.join();
    c5.join();
    g2.join();


    std::cout << "threads er done" << std::endl;

    return 0;
}
