#include <MemoryManager.h>

typedef struct MemoryManagerCDT {
	uint8_t * managed_memory_start_address;
} MemoryManagerCDT;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
	MemoryManagerADT the_mman = (MemoryManagerADT) memoryForMemoryManager;
	bmp_initialize((uint8_t *)memoryForMemoryManager+0x100, MMAN_MEMSIZE/MMAN_PAGESIZE);
	the_mman->managed_memory_start_address = (uint8_t *) managedMemory;
	return the_mman;
}

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate) {
	bit_index bit_idx;
	slot_index slot_idx;
	size_t pages_needed = memoryToAllocate%MMAN_PAGESIZE == 0 ? 
							memoryToAllocate/MMAN_PAGESIZE : (memoryToAllocate/MMAN_PAGESIZE)+1;
	bit_amount start_page = bmp_find(pages_needed, &slot_idx, &bit_idx);
	bmp_set_on(pages_needed, start_page);

	return (void *)(memoryManager->managed_memory_start_address + (slot_idx*BITS_PER_SLOT + bit_idx)*MMAN_PAGESIZE);
}
