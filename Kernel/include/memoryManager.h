#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#define MMAN_FREE 0
#define MMAN_USED 1
#define MMAN_BOUNDARY 2

#define MMAN_PAGESIZE 0x1000
#define MMAN_MEMSIZE 0xFF00000
#define MMAN_PAGECOUNT (MMAN_MEMSIZE / MMAN_PAGESIZE)

#define MMAN_PAGESIZE 0x1000
#define MMAN_PAGECOUNT (MMAN_MEMSIZE / MMAN_PAGESIZE)

#include "bitmap.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct memoryManager *memoryManagerADT;

memoryManagerADT
create_memory_manager(void *const restrict memory_for_memory_manager,
                      void *const restrict managed_memory);

void *alloc_memory(memoryManagerADT const restrict memory_manager,
                   const size_t memory_to_allocate);

void mman_free(memoryManagerADT const restrict memory_manager, void *ptr);

void print_mem(memoryManagerADT const restrict memory_manager);

#endif
