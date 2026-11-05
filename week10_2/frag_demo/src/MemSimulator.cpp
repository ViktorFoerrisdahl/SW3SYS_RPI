#include "MemSimulator.hpp"
#include <vector>
#include <cstddef>
#include <iostream>

MemSimulator::MemSimulator(size_t totalSize) {
  blocks.push_back({totalSize, true});
}

void MemSimulator::allocate(size_t size) {
  for (size_t i = 0; i < blocks.size(); ++i) {
    if (blocks[i].free && blocks[i].size >= size) {
      size_t remaining = blocks[i].size - size;

      blocks[i].size = size;
      blocks[i].free = false;

      if (remaining > 0) {
        blocks.insert(blocks.begin() + i + 1,
                      {remaining, true});
      }
      return;
    }
  }
  std::cout << "Allocation failed\n";
}

void MemSimulator::deallocate(size_t index) {
  if (index < blocks.size()) {
    blocks[index].free = true;
  }
}

void MemSimulator::showLayout() {
  std::cout << "Memory layout: ";

  for (const auto& block : blocks) {
    if (block.free)
      std::cout << "[FREE:" << block.size << "] ";
    else
      std::cout << "[USED:" << block.size << "] ";
  }

  std::cout << "\n";
}

void MemSimulator::showVisual() {
  std::cout << "Memory layout: ";

  for (const auto& block : blocks) {
    char symbol = block.free ? '.' : '#';

    for (size_t i = 0; i < block.size; ++i) {
      std::cout << symbol;
    }
  }

  std::cout << "\n";
}
