#ifndef MEMSIMULATOR_HPP
#define MEMSIMULATOR_HPP

#include <vector>
#include <cstddef>

struct Block {
  size_t size;
  bool free;
};

class MemSimulator {
  private:
    std::vector<Block> blocks;

  public:
    MemSimulator(size_t totalSize);

    void allocate(size_t size);
    void deallocate(size_t index);
    void showLayout();
    void showVisual();
};

#endif
