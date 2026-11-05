#include "MemoryPool.hpp"
#include <chrono>
#include <iostream>

struct Family {
  std::vector<std::string> names;
  std::vector<int> ages;
};

int main(int argc, char* argv[]) {
  const int N = 1000000;

  /* ---- Using new/delete ---- */
  auto start1 = std::chrono::steady_clock::now();

  for (int i = 0; i < N; ++i) {
    Family* f = new Family;
    std::string name = "John Smith";
    int age = 43;
    f->names.push_back(name);
    f->ages.push_back(age);
    delete f;
  }

  auto end1 = std::chrono::steady_clock::now();

  /* ---- Using memory pool ---- */
  MemoryPool pool(sizeof(Family), N);

  auto start2 = std::chrono::steady_clock::now();

  for (int i = 0; i < N; ++i) {
    Family* f = new (pool.allocate()) Family;
    std::string name = "John Smith";
    int age = 43;
    f->names.push_back(name);
    f->ages.push_back(age);
    pool.deallocate(f);
  }

  auto end2 = std::chrono::steady_clock::now();

  auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
  auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);

  std::cout << "new/delete: " << duration1.count() << " ms\n";
  std::cout << "memory pool: " << duration2.count() << " ms\n";

}
