// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "syscall.h"
#include "test_util.h"

enum State { RUNNING, BLOCKED, KILLED };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

int64_t test_processes(int argc, char **argv) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes = satoi(argv[0]);
  char *argvAux[] = {0};

  if (argc != 1)
    return -1;

  if (max_processes == 0 || max_processes > 20)
    max_processes = 5;

  p_rq p_rqs[max_processes];

  printf("Max processes per loop: %d\n", (int)max_processes);

  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      argvAux[0] = "&";
      p_rqs[rq].pid = my_create_process("endless_loop", 1, argvAux);

      printf("Created PID: %d\n", (int)p_rqs[rq].pid);

      sleepms(1);

      if (p_rqs[rq].pid == -1) {
        printf("test_processes: ERROR creating process\n");
        return -1;
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been
    // killed
    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
        case 0:
          if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
            if (my_kill(p_rqs[rq].pid) == -1) {
              printf("test_processes: ERROR killing process\n");
              return -1;
            }
            printf("Killed PID: %d\n", (int)p_rqs[rq].pid);
            p_rqs[rq].state = KILLED;
            alive--;
          }
          break;

        case 1:
          if (p_rqs[rq].state == RUNNING) {
            if (my_block(p_rqs[rq].pid) == -1) {
              printf("test_processes: ERROR blocking process\n");
              return -1;
            }
            printf("Blocked PID: %d\n", (int)p_rqs[rq].pid);
            p_rqs[rq].state = BLOCKED;
          }
          break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (my_unblock(p_rqs[rq].pid) == -1) {
            printf("test_processes: ERROR unblocking process\n");
            return -1;
          }
          printf("Unblocked PID: %d\n", (int)p_rqs[rq].pid);
          p_rqs[rq].state = RUNNING;
        }
    }
  }

  printf("\nTest ended. Processes were not printed because\n");
  printf("they were created at background.\n");
  return 0;
}
