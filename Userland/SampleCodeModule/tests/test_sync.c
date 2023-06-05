// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "syscall.h"
#include "test_util.h"
#include <stdio.h>

#define SEM_ID "sem_sem"
#define TOTAL_PAIR_PROCESSES 4

int64_t global; // shared memory
void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  my_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

void my_process_inc(int argc, char *argv[]) {

  int64_t n = satoi(argv[1]);
  int8_t inc = satoi(argv[2]);
  int8_t use_sem = satoi(argv[3]);

  int my_sem = -1;

  if (argc != 4)
    return;

  if (n <= 0 || n > 10)
    n = 4;

  if (use_sem) {
    if ((my_sem = my_sem_open(SEM_ID, 1)) == -1) {
      printf("test_sync: ERROR opening semaphore\n");
      return;
    }
  }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem) {
      my_sem_wait(my_sem);
    }
    slowInc(&global, inc);
    if (use_sem) {
      my_sem_post(my_sem);
    }
  }

  return;
}

uint64_t test_sync(int argc, char *argv[]) { //{n, use_sem}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)
    return -1;

  char *argvDec[] = {"Decreaser", argv[0], "-1", argv[1]};
  char *argvInc[] = {"Increaser", argv[0], "1", argv[1]};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = call_to_create_process(4, argvDec, my_process_inc, NULL);
    pids[i + TOTAL_PAIR_PROCESSES] =
        call_to_create_process(4, argvInc, my_process_inc, NULL);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    my_wait(pids[i]);
    my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  printf("Expected value: 0\n");
  printf("Final value: %d\n", (int)global);

  return 0;
}
