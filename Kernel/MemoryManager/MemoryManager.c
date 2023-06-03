#include <MemoryManager.h>

typedef struct page_info {
  uint8_t state;
} page_info;

typedef struct MemoryManager {
  uint8_t *managed_memory_start_address;
  page_info page_info_array[MMAN_PAGECOUNT];
} MemoryManagerCDT;

MemoryManagerADT
create_memory_manager(void *const restrict memory_for_memory_manager,
                      void *const restrict managed_memory) {
  MemoryManagerADT the_mman = (MemoryManagerADT)memory_for_memory_manager;
  if (!bmp_initialize((uint8_t *)memory_for_memory_manager +
                          sizeof(MemoryManagerCDT),
                      MMAN_PAGECOUNT))
    return NULL;
  the_mman->managed_memory_start_address = (uint8_t *)managed_memory;
  for (int i = 0; i < MMAN_PAGECOUNT; i++) {
    the_mman->page_info_array[i].state = MMAN_FREE;
  }
  return the_mman;
}

void *alloc_memory(MemoryManagerADT const restrict memory_manager,
                   const size_t memory_to_allocate) {
  bit_index bit_idx;
  slot_index slot_idx;
  size_t pages_needed = memory_to_allocate % MMAN_PAGESIZE == 0
                            ? memory_to_allocate / MMAN_PAGESIZE
                            : (memory_to_allocate / MMAN_PAGESIZE) + 1;
  bit_amount start_page = bmp_find(pages_needed, &slot_idx, &bit_idx);
  if (start_page == -1)
    return NULL;
  bmp_set_on(pages_needed, start_page);

  int limit = start_page + pages_needed;

  memory_manager->page_info_array[start_page].state = MMAN_BOUNDARY;
  for (int i = start_page + 1; i < limit; i++) {
    memory_manager->page_info_array[i].state = MMAN_USED;
  }

  return (void *)(memory_manager->managed_memory_start_address +
                  (slot_idx * BITS_PER_SLOT + bit_idx) * MMAN_PAGESIZE);
}

void mman_free(MemoryManagerADT const restrict memory_manager, void *ptr) {
  if (ptr == NULL)
    return;
  uint32_t start_page =
               ((uint64_t)ptr -
                (uint64_t)memory_manager->managed_memory_start_address) /
               (MMAN_PAGESIZE),
           curr_page = start_page;
  page_info *pages_info = memory_manager->page_info_array;
  do {
    pages_info[curr_page].state = MMAN_FREE;
    curr_page++;
  } while (pages_info[curr_page].state == MMAN_USED);
  bmp_set_off(curr_page - start_page, start_page);
}