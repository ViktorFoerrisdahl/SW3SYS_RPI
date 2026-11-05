#ifndef MEMORYPOOL_HPP
#define MEMORYPOOL_HPP

#include <cstddef>
#include <vector>

class MemoryPool {
  public:
    MemoryPool(size_t blockSize, size_t numBlocks);
    ~MemoryPool();

    void* allocate();
    void deallocate(void* ptr);

  private:
    size_t blockSize; // size of each memory block
    size_t numBlocks; // total number of blocks in memory pool
    char* pool; // pointer to one large chunk of memory
    std::vector<void*> freeList; // list of pointers to free memory blocks
};

#endif
