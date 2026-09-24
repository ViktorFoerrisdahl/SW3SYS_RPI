#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <random>
#include <unistd.h>

void printInfo(int ID)
{
    int counter{1};
    for (auto i = 0; i < 10; i++)
    {
        std::cout << "mit ID er: " << ID << " og antallet af print: " << counter << std::endl;
        counter++;
        sleep(1);
    }
}

int main ()
{

    std::thread t1(printInfo,1);
    std::thread t2(printInfo,2);

    t1.join();
    t2.join();

    return 0;
}