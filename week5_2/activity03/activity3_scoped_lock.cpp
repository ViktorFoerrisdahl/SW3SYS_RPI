#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <random>
#include <unistd.h>
#include <mutex>

class Guard 
{
    public:
    Guard (std::mutex &lock) : lock_(&lock)
    {
        lock_->lock();
    }

    ~Guard ()
    {
        lock_->unlock();
    }

    private:
    std::mutex *lock_;
};

void printInfo(int ID, std::mutex &lock)
{
    Guard g(lock);

    for (auto i = 1; i < 11; i++)
    {
        std::cout << "mit ID er: " << ID << " og antallet af print: " << i << std::endl;
        sleep(1);
    }
}

int main ()
{
    std::mutex lock1;

    std::thread t1(printInfo, 1, std::ref(lock1));
    std::thread t2(printInfo, 2, std::ref(lock1));

    t1.join();
    t2.join();

    return 0;
}