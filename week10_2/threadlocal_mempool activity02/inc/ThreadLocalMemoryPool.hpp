#ifndef THREADLOCALMEMORYPOOL_HPP
#define THREADLOCALMEMORYPOOL_HPP

#include <cstddef>
#include <mutex>
#include <vector>

class ThreadLocalMemoryPool {
  public:
    ThreadLocalMemoryPool(size_t blockSize, size_t numBlocks);
    ~ThreadLocalMemoryPool();

    void* allocate();
    void deallocate(void* ptr);

  private:
    size_t blockSize; // size of each memory block
    size_t numBlocks; // total number of blocks in memory pool
    char* pool; // pointer to one large chunk of memory
    std::vector<void*> freeList; // list of pointers to free memory blocks
    static thread_local std::vector<void*> localFreeList; // Thread-local list of pointers
    std::mutex mtx; // Mutex for concurrency
};

#endif
