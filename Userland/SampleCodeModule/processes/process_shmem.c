#include "include/process_shmem.h"

#define SHMEM_SIZE 4096

#define SHMEM_NAME "test_shmem"

int cap = 0;

void shmem_writer(int argc, char *argv[]) {

  // conectarnos a la shmemem
  uintptr_t *shmem_start = call_to_shmem_open(SHMEM_NAME);

  while (1) {
    shmem_start[0] = 'a' + cap;
    cap++;
    call_to_sleep(500);
  }
}

void shmem_reader(int argc, char *argv[]) {

  // conectarnos a la shmem
  uintptr_t *shmem_start = call_to_shmem_open(SHMEM_NAME);

  while (1) {
    printf("%c", (char)shmem_start[1]);

    printf("\n");
    call_to_sleep(500);
  }
}