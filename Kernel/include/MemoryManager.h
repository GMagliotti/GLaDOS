#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#define MMAN_FREE 0
#define MMAN_USED 1
#define MMAN_BOUNDARY 2 


#define MMAN_PAGESIZE   0x1000
#define MMAN_MEMSIZE    0x10000000
#define MMAN_PAGECOUNT  (MMAN_MEMSIZE/MMAN_PAGESIZE)

#include <stdlib.h>
#include <bitmap.h>
#include <lib.h>

typedef struct MemoryManagerCDT *MemoryManagerADT;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate);

void mman_free(MemoryManagerADT const restrict memoryManager, void * ptr);

#endif
