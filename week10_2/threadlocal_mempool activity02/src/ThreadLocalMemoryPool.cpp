#include "ThreadLocalMemoryPool.hpp"
#include <vector>
#include <mutex>

// Define thread-local memory pool
thread_local std::vector<void*> ThreadLocalMemoryPool::localFreeList;

// Constructor
ThreadLocalMemoryPool::ThreadLocalMemoryPool(size_t blockSize, size_t numBlocks)
            : blockSize(blockSize), numBlocks(numBlocks) {
  // Allocate one big contiguous chunk of memory
  pool = new char[blockSize * numBlocks];

  // Add each block's address to freeList
  for (size_t i = 0; i < numBlocks; ++i) {
    freeList.push_back(pool + i * blockSize);
  }
}

// Destructor
ThreadLocalMemoryPool::~ThreadLocalMemoryPool() {
  // Free entire memory pool when class object is destroyed
  delete[] pool;
}

// Take out a memory block from pool
void* ThreadLocalMemoryPool::allocate() {
  // If thread-local memory block has free memory, use it
  if (!localFreeList.empty()) {
    void* ptr = localFreeList.back();
    localFreeList.pop_back();
    return ptr;
  }

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
void ThreadLocalMemoryPool::deallocate(void* ptr) {
  localFreeList.push_back(ptr);

  if (localFreeList.size() > 64) {
    std::lock_guard<std::mutex> lock(mtx);
    freeList.push_back(localFreeList.back());
    localFreeList.pop_back();
  }
}
