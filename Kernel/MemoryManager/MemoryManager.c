#include <MemoryManager.h>

typedef struct page_info {
	uint8_t state;
} page_info;

typedef struct MemoryManager {
	uint8_t * managed_memory_start_address;
	page_info page_info_array[MMAN_PAGECOUNT];
} MemoryManagerCDT;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
	MemoryManagerADT the_mman = (MemoryManagerADT) memoryForMemoryManager;
	if (!bmp_initialize((uint8_t *)memoryForMemoryManager+sizeof(MemoryManagerCDT), MMAN_PAGECOUNT)) return NULL;
	the_mman->managed_memory_start_address = (uint8_t *) managedMemory;
	return the_mman;
}

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate) {
	bit_index bit_idx;
	slot_index slot_idx;
	size_t pages_needed = memoryToAllocate%MMAN_PAGESIZE == 0 ? 
							memoryToAllocate/MMAN_PAGESIZE : (memoryToAllocate/MMAN_PAGESIZE)+1;
	bit_amount start_page = bmp_find(pages_needed, &slot_idx, &bit_idx);
	if (start_page == -1) return NULL;
	bmp_set_on(pages_needed, start_page);
	
	int limit = start_page + pages_needed;

	memoryManager->page_info_array[start_page].state = MMAN_BOUNDARY;
	for (int i = start_page; i < limit; i++) {
		memoryManager->page_info_array[i].state = MMAN_USED;
	}

	return (void *) (memoryManager->managed_memory_start_address + (slot_idx*BITS_PER_SLOT + bit_idx)*MMAN_PAGESIZE);
}

void mman_free(MemoryManagerADT const restrict memoryManager, void * ptr) {
	if (ptr == NULL) return;
	uint32_t start_page = ((uint64_t)ptr - (uint64_t)memoryManager->managed_memory_start_address)/(MMAN_PAGESIZE), 
				curr_page = start_page;
	page_info * pages_info = memoryManager->page_info_array;
	do {
		pages_info[curr_page].state = MMAN_FREE;
		curr_page++;
	} while ( pages_info[curr_page].state != MMAN_BOUNDARY );
	bmp_set_off(curr_page-start_page, start_page);
}