/* buddy.c
 *
 * Description:
 *   Implement buddy system for memory management
 *
 * Idea & part of the code are from https://github.com/wuwenbin/buddy2
 */
#include "../include/memoryManager.h"

struct memoryManager {
  size_t size;
  void *start;
  // TODO what the fuck is this
  // size 1????
  size_t longest[MMAN_PAGECOUNT];
};

static inline int left_child(int index) {
  /* index * 2 + 1 */
  return ((index << 1) + 1);
}

static inline int right_child(int index) {
  /* index * 2 + 2 */
  return ((index << 1) + 2);
}

static inline int parent(int index) {
  /* (index+1)/2 - 1 */
  return (((index + 1) >> 1) - 1);
}

static inline bool is_power_of_2(int index) { return !(index & (index - 1)); }

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

static inline unsigned next_power_of_2(unsigned size) {
  /* depend on the fact that size < 2^32 */
  size -= 1;
  size |= (size >> 1);
  size |= (size >> 2);
  size |= (size >> 4);
  size |= (size >> 8);
  size |= (size >> 16);
  return size + 1;
}

/** allocate a new buddy structure
 * @param num_of_fragments number of fragments of the memory to be managed
 * @return pointer to the allocated buddy structure */
struct memoryManager *buddy_new(void *const restrict memory_for_memory_manager,
                                unsigned num_of_fragments,
                                void *managed_memory) {
  struct memoryManager *self = NULL;
  size_t node_size;

  int i;

  if (num_of_fragments < 1 || !is_power_of_2(num_of_fragments)) {
    return NULL;
  }

  /* allocate an array to represent a complete binary tree */
  // self = (struct buddy *) b_malloc(sizeof(struct buddy)
  //                                  + 2 * num_of_fragments * sizeof(size_t));
  self = (struct memoryManager *)memory_for_memory_manager;
  self->start = managed_memory;
  self->size = num_of_fragments;
  node_size = num_of_fragments * 2;

  /* initialize *longest* array for buddy structure */
  int iter_end = num_of_fragments * 2 - 1;
  for (i = 0; i < iter_end; i++) {
    if (is_power_of_2(i + 1)) {
      node_size >>= 1;
    }
    self->longest[i] = node_size;
  }

  return self;
}

/* choose the child with smaller longest value which is still larger
 * than *size* */
unsigned choose_better_child(struct memoryManager *self, unsigned index,
                             size_t size) {
  struct compound {
    size_t size;
    unsigned index;
  } children[2];
  children[0].index = left_child(index);
  children[0].size = self->longest[children[0].index];
  children[1].index = right_child(index);
  children[1].size = self->longest[children[1].index];

  int min_idx = (children[0].size <= children[1].size) ? 0 : 1;

  if (size > children[min_idx].size) {
    min_idx = 1 - min_idx;
  }

  return children[min_idx].index;
}

/** allocate *size* from a buddy system *self*
 * @return the offset from the beginning of memory to be managed */
int buddy_alloc(struct memoryManager *self, size_t size) {
  if (self == NULL || self->size < size) {
    return -1;
  }
  size = next_power_of_2(size);

  unsigned index = 0;
  if (self->longest[index] < size) {
    return -1;
  }

  /* search recursively for the child */
  unsigned node_size = 0;
  for (node_size = self->size; node_size != size; node_size >>= 1) {
    /* choose the child with smaller longest value which is still larger
     * than *size* */
    /* TODO */
    index = choose_better_child(self, index, size);
  }

  /* update the *longest* value back */
  self->longest[index] = 0;
  int offset = (index + 1) * node_size - self->size;

  while (index) {
    index = parent(index);
    self->longest[index] = max(self->longest[left_child(index)],
                               self->longest[right_child(index)]);
  }

  return offset;
}

void buddy_free(struct memoryManager *self, int offset) {
  if (self == NULL || offset < 0 || offset > (int)self->size) {
    return;
  }

  size_t node_size;
  unsigned index;

  /* get the corresponding index from offset */
  node_size = 1;
  index = offset + self->size - 1;

  for (; self->longest[index] != 0; index = parent(index)) {
    node_size <<= 1; /* node_size *= 2; */

    if (index == 0) {
      break;
    }
  }

  self->longest[index] = node_size;

  while (index) {
    index = parent(index);
    node_size <<= 1;

    size_t left_longest = self->longest[left_child(index)];
    size_t right_longest = self->longest[right_child(index)];

    if (left_longest + right_longest == node_size) {
      self->longest[index] = node_size;
    } else {
      self->longest[index] = max(left_longest, right_longest);
    }
  }
}

void buddy_dump(struct memoryManager *self) {
  int len = self->size << 1;
  int max_col = self->size << 1;
  int level = 0;
  // int i, j;
  int i;

  // char cs[] = {'/', '\\'};
  // int idx = 0;
  // char c;

  print_string(
      "---------------------------------------------------------------\n", 64);
  print_color_string("Buddy tree representation\n", 0x100, 0xd503ff);

  print_color_string("N = max number of pages available on the node\n", 0x100,
                     0x00FF00);

  print_color_string("PAGESIZE = 4096B\n", 0x100, 0x0364ff);

  for (i = 0, max_col = len, level = 0; i < len - 1; i++) {
    if (is_power_of_2(i + 1)) {
      max_col >>= 1;
      level++;
      // idx = 0;
      sleep(2);

      print_string("\n|| Level: ", 16);
      print_number(level, 10);
      print_string(": ", 16);
    }

    print_number((int)self->longest[i], 10);
    print_string(" - ", 10);
  }

  print_string("\n------------------------------------------------------------"
               "---\n",
               66);
}

int buddy_size(struct memoryManager *self, int offset) {
  unsigned node_size = 1;
  unsigned index = offset + self->size - 1;

  for (; self->longest[index]; index = parent(index)) {
    node_size >>= 1;
  }

  return node_size;
}

// WRAPPERS

memoryManagerADT
create_memory_manager(void *const restrict memory_for_memory_manager,
                      void *const restrict managed_memory) {
  unsigned int size = MMAN_MEMSIZE;
  size = is_power_of_2(size) ? size : (next_power_of_2(size) / 2);
  return buddy_new(memory_for_memory_manager, size / MMAN_PAGESIZE,
                   managed_memory);
}

void *alloc_memory(memoryManagerADT const restrict memory_manager,
                   const size_t memory_to_allocate) {
  int offset =
      buddy_alloc(memory_manager, (memory_to_allocate % MMAN_PAGESIZE == 0)
                                      ? memory_to_allocate / MMAN_PAGESIZE
                                      : memory_to_allocate / MMAN_PAGESIZE + 1);
  if (offset == -1)
    return NULL;
  return (void *)((char *)memory_manager->start + offset * MMAN_PAGESIZE);
}

void mman_free(struct memoryManager *const restrict memory_manager, void *ptr) {
  buddy_free(memory_manager,
             ((char *)ptr - (char *)memory_manager->start) / MMAN_PAGESIZE);
}

void print_mem(memoryManagerADT const restrict memory_manager) {
  buddy_dump(memory_manager);
}
