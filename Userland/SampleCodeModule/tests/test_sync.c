#include "test_util.h"

#define SEM_ID "sem_sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory
int my_sem;

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  call_to_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

void my_process_inc(int argc, char *argv[]) {
  uint64_t n = 2;
  int8_t inc = 1;
  int8_t use_sem = true;

  // if (argc != 3)
  //   return -1;

  // if ((n = satoi(argv[1])) <= 0)
  //   return -1;
  // if ((inc = satoi(argv[2])) == 0)
  //   return -1;
  if ((use_sem = satoi(argv[3])) < 0)
    return;

  if (use_sem)
    // if (!my_sem_open(SEM_ID, 1)) {
    if ((my_sem = call_to_sem_open(SEM_ID)) == -1) {
      if ((my_sem = call_to_create_sem(1, SEM_ID)) == -1) {
        printf("test_sync: ERROR opening semaphore\n");
        return;
      }
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem) {
      printf("Catching the sem: %d\n", call_to_getpid());
      call_to_sem_wait(my_sem);
      // my_sem_wait(SEM_ID);
    }
    printf("Caught: %d\n", call_to_getpid());
    slowInc(&global, inc);
    if (use_sem) {
      printf("Lifting the sem: %d\n", call_to_getpid());
      call_to_sem_post(my_sem);
      // my_sem_post(SEM_ID);
    }
  }

  // if (use_sem) {
  //   call_to_destroy_sem(my_sem);
  //   // my_sem_close(SEM_ID);
  // }

  return;
}

uint64_t test_sync(int argc, char *argv[]) { //{n, use_sem, 0}
  // const char *amount = "4";
  char *amount = "4";
  uint64_t pair_proc_num = satoi(amount);
  uint64_t pids[2 * pair_proc_num];

  // if (argc != 3)
  //   return -1;

  if ((satoi(argv[2])) == 1)
    printf("Con semaforos:\n");
  else
    printf("Sin semaforos:\n");

  // char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  // char *argvInc[] = {argv[0], "1", argv[1], NULL};
  char *argvDec[] = {"Decock", "1", "-1", argv[2]};
  char *argvInc[] = {"Incock", "1", "1", argv[2]};

  global = 0;

  uint64_t i;
  for (i = 0; i < pair_proc_num; i++) {
    pids[i] = call_to_create_process(4, argvDec, my_process_inc, NULL);
    pids[i + pair_proc_num] =
        call_to_create_process(4, argvInc, my_process_inc, NULL);
  }

  for (i = 0; i < pair_proc_num * 2; i++) {
    // my_wait(pids[i]);
    // my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
    call_to_setSize(1);
    // call_to_clear_screen();
    printf("Esperando las creatura\n");
    // call_to_ps();
    call_to_waitpid(pids[i]);
  }

  printf("Final value: %d\n", global);

  return 0;
}
