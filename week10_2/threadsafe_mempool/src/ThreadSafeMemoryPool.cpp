#include "ThreadSafeMemoryPool.hpp"

// Constructor
ThreadSafeMemoryPool::ThreadSafeMemoryPool(size_t blockSize, size_t numBlocks)
            : blockSize(blockSize), numBlocks(numBlocks) {
  // Allocate one big contiguous chunk of memory
  pool = new char[blockSize * numBlocks];

  // Add each block's address to freeList
  for (size_t i = 0; i < numBlocks; ++i) {
    freeList.push_back(pool + i * blockSize);
  }
}

// Destructor
ThreadSafeMemoryPool::~ThreadSafeMemoryPool() {
  // Free entire memory pool when class object is destroyed
  delete[] pool;
}

// Take out a memory block from pool
void* ThreadSafeMemoryPool::allocate() {
  std::lock_guard<std::mutex> lock(mtx);

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
void ThreadSafeMemoryPool::deallocate(void* ptr) {
  std::lock_guard<std::mutex> lock(mtx);

  // Add pointer to memory block back into list
  freeList.push_back(ptr);
}
