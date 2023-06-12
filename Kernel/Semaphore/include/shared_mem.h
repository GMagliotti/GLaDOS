#include <stdint.h>

#define SHMEM_SIZE 4096

uintptr_t *shmem_open(const char *name);