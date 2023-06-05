// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_util.h"
#include <stdint.h>

#define MAX_BLOCKS 128

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

uint64_t test_mm(int argc, char **argv) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint64_t max_memory = satoi(argv[0]);

  if (max_memory == 0 || max_memory > TEST_MAXMEM)
    max_memory = TEST_MAXMEM;

  printf("Memory requested (capped at %d): %d\n", TEST_MAXMEM, (int)max_memory);
  while (1) {
    uint8_t rq = 0;
    uint32_t total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      printf("Size: %d - ", (int)mm_rqs[rq].size);
      mm_rqs[rq].address = call_to_malloc(mm_rqs[rq].size);
      printf("Addr: %x\n", (uint64_t)mm_rqs[rq].address);

      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          printf("test_mm ERROR\n");
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address) {
        printf("Free addr: %x\n", (uint64_t)mm_rqs[i].address);
        call_to_free(mm_rqs[i].address);
      }
  }
}
