#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <random>
#include <unistd.h>
#include <mutex>
#include <semaphore>

void printInfo(int ID, std::binary_semaphore &sem)
{

    sem.acquire();

    for (auto i = 1; i < 11; i++)
    {
        std::cout << "mit ID er: " << ID << " og antallet af print: " << i << std::endl;
        sleep(1);
    }

    sem.release();
}

int main ()
{

    std::binary_semaphore sem{1};

    std::thread t1(printInfo, 1, std::ref(sem));
    std::thread t2(printInfo, 2, std::ref(sem));

    t1.join();
    t2.join();

    return 0;
}