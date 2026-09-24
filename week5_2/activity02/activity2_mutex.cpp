#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <random>
#include <unistd.h>
#include <mutex>

void printInfo(int ID, std::mutex &lock)
{
        
    lock.lock();

    for (auto i = 1; i < 11; i++)
    {
        std::cout << "mit ID er: " << ID << " og antallet af print: " << i << std::endl;
        sleep(1);
    }
       
    lock.unlock();

}

int main ()
{

    std::mutex lock;

    std::thread t1(printInfo,1,std::ref(lock));
    std::thread t2(printInfo,2,std::ref(lock));

    t1.join();
    t2.join();

    return 0;
}