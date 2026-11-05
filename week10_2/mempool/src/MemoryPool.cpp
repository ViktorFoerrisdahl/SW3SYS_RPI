#include "MemoryPool.hpp"

// Constructor
MemoryPool::MemoryPool(size_t blockSize, size_t numBlocks)
            : blockSize(blockSize), numBlocks(numBlocks) {
  // Allocate one big contiguous chunk of memory
  pool = new char[blockSize * numBlocks];

  // Add each block's address to freeList
  for (size_t i = 0; i < numBlocks; ++i) {
    freeList.push_back(pool + i * blockSize);
  }
}

// Destructor
MemoryPool::~MemoryPool() {
  // Free entire memory pool when class object is destroyed
  delete[] pool;
}

// Take out a memory block from pool
void* MemoryPool::allocate() {
  // If no free blocks remain, throw allocation error
  if (freeList.empty()) { throw std::bad_alloc(); }

  // Get last available block from list
  void* ptr = freeList.back();
  // Remove it from list (marking it as “in use”)
  freeList.pop_back();
  // returns pointer to that block
  return ptr;
}

// Return assigned memory block back to pool
void MemoryPool::deallocate(void* ptr) {
  // Add pointer to memory block back into list
  freeList.push_back(ptr);
}
