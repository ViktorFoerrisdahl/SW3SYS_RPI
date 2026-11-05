#include "ThreadLocalMemoryPool.hpp"
#include <thread>
#include <iostream>
#include <unistd.h>

struct Family
{
  std::vector<std::string> names;
  std::vector<int> ages;
};

int main(int argc, char *argv[])
{
  ThreadLocalMemoryPool pool(sizeof(Family), 10000000); // Instantiate memory pool

  // Function executed by threads
  auto worker = [&pool](int nr) { // Lambda function that captures pool by reference
    std::cout << "Thread running...\n";
    std::vector<Family*> fam_pointers;
    for (int i = 0; i < 5000; ++i)
    {
      Family *f = new (pool.allocate()) Family; // Allocate memory + construct object
      std::string name = "fam " + std::to_string(i) + " in thread: " + std::to_string(nr);
      f->names.push_back(name); // Write data to
      fam_pointers.push_back(f);
    }

    while (1)
    {
      int randomNum = rand() % 25;
      std::cout << fam_pointers[randomNum]->names[0] << "\n";
      sleep(nr);
    }

    std::cout << "Thread finished.\n";
  };

  // Spawn threads
  std::thread t1(worker,2);
  std::thread t2(worker,1);

  // Wait until threads finish
  t1.join();
  t2.join();

  std::cout << "All threads completed.\n";
  return 0;
}
