#include "MemoryPool.hpp"
#include <iostream>

struct Family {
  std::vector<std::string> names;
  std::vector<int> ages;
};

int main(int argc, char* argv[]) {
  MemoryPool pool(sizeof(Family), 100); // Instantiate memory pool to hold 100 structs

  Family* f = new (pool.allocate()) Family; // Allocate memory + construct Family object

  std::string name = "John Smith";
  int age = 43;
  f->names.push_back(name); // Write data to
  f->ages.push_back(age);   // allocated memory
  std::cout << f->names[0] << ", " << f->ages[0] << "\n";
  f->~Family();       // Destroy Family object
  pool.deallocate(f); // Return memory to pool
  return 0;
}
