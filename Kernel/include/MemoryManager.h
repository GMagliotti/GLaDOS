#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#define MMAN_PAGESIZE   0x1000
#define MMAN_START_ADDR 0x1000000
#define MMAN_MEMSIZE    0x1000000

#include <stdlib.h>
#include <bitmap.h>

typedef struct MemoryManagerCDT *MemoryManagerADT;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate);

#endif