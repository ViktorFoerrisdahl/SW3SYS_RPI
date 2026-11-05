#include "ThreadLocalMemoryPool.hpp"
#include <thread>
#include <iostream>

struct Family {
  std::vector<std::string> names;
  std::vector<int> ages;
};

int main(int argc, char* argv[]) {
  ThreadLocalMemoryPool pool(sizeof(Family), 10); // Instantiate memory pool

  // Function executed by threads
  auto worker = [&pool]() { // Lambda function that captures pool by reference
    std::cout << "Thread running...\n";
    int blkCnt = 0; // Counter for number of memory allocations by the thread

    for (int i = 0; i < 50; ++i) {
      Family* f = new (pool.allocate()) Family; // Allocate memory + construct object
      std::string name = "John Smith";
      int age = 43;
      f->names.push_back(name); // Write data to
      f->ages.push_back(age);   // allocated memory
      f->~Family();             // Destroy Family object
      pool.deallocate(f);       // Deallocate memory
      std::cout << "Blocks = " << ++blkCnt << "\n";
    }
    std::cout << "Thread finished.\n";
  };

  // Spawn threads
  std::thread t1(worker);
  std::thread t2(worker);
  std::thread t3(worker);

  // Wait until threads finish
  t1.join();
  t2.join();
  t3.join();

  std::cout << "All threads completed.\n";
  return 0;
}
