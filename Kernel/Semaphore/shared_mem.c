#include "./include/shared_mem.h"
#include <stdlib.h>
#include <textDriver.h>

#define SHMEM_DATA_ARRAY_SIZE 32

typedef struct shmem_spaces {
  uintptr_t *start_address;
  char name[128];
} shmem_data;

shmem_data shmem_data_array[SHMEM_DATA_ARRAY_SIZE] = {0};
static int current_create_pos = 0;

uintptr_t *shmem_open(const char *name) {
  for (int i = 0; i < SHMEM_DATA_ARRAY_SIZE; i++) {
    if (string_equals(name, shmem_data_array[i].name)) {
      return shmem_data_array[i].start_address;
    }
  }
  shmem_data_array[current_create_pos].start_address = sys_malloc(SHMEM_SIZE);
  str_cpy(shmem_data_array[current_create_pos].name, name);
  return shmem_data_array[current_create_pos++].start_address;
}