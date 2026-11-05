#ifndef THREADSAFEMEMORYPOOL_HPP
#define THREADSAFEMEMORYPOOL_HPP

#include <cstddef>
#include <mutex>
#include <vector>

class ThreadSafeMemoryPool {
  public:
    ThreadSafeMemoryPool(size_t blockSize, size_t numBlocks);
    ~ThreadSafeMemoryPool();

    void* allocate();
    void deallocate(void* ptr);

  private:
    size_t blockSize; // size of each memory block
    size_t numBlocks; // total number of blocks in memory pool
    char* pool; // pointer to one large chunk of memory
    std::vector<void*> freeList; // list of pointers to free memory blocks
    std::mutex mtx;
};

#endif
