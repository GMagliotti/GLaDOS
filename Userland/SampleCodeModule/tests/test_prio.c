// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_util.h"

#define MINOR_WAIT                                                             \
  10000 // TODO: Change this value to prevent a process from flooding the
        // screen
#define WAIT                                                                   \
  1000000 // TODO: Change this value to make the wait long enough to see theese
          // processes beeing run at least twice

#define TOTAL_PROCESSES 5
#define LOWEST 0
#define MEDIUM 3
#define HIGHEST 5

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST, MEDIUM, LOWEST};

void test_prio() {
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {0};
  uint64_t i;

  argv[0] = "startfull_loop_print";
  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = call_to_create_process(1, argv, endless_loop_print, NULL);

  bussy_wait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], prio[i]);

  bussy_wait(WAIT);
  printf("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_block(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], MEDIUM);

  printf("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_unblock(pids[i]);

  bussy_wait(WAIT);
  printf("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_kill(pids[i]);

  printf("\nTest ended. Processes were not printed because\n");
  printf("they were created at background.\n");
}
