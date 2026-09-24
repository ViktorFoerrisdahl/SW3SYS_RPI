#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <random>
#include <unistd.h>
#include <mutex>

class printClaas
{
private:
    inline static std::mutex lock_;
    int id_;

public:
    printClaas(int ID) : id_(ID) {}

    void printInfo()
    {
        this->lock_.lock();

        for (auto i = 1; i < 11; i++)
        {
            std::cout << "mit ID er: " << this->id_ << " og antallet af print: " << i << std::endl;
            sleep(1);
        }
        this->lock_.unlock();
    }

    void locklock()
    {
        this->lock_.lock();
    }
};

int main()
{
    printClaas A(1);
    printClaas B(2);

    std::thread t1([&](){A.printInfo();});
    std::thread t2([&](){B.printInfo();});

    // A.locklock(); danner deadlock efter a har udprintet hele sit std::cout

    t1.join();
    t2.join();

    return 0;
}