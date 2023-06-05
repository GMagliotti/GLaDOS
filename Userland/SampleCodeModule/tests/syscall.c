// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "syscall.h"
#include "test_util.h"

int64_t my_getpid() { return call_to_getpid(); }

int64_t my_create_process(char *name, uint64_t argc, char *argv[]) {

  for (int i = argc; i > 0; i--) {
    argv[i] = argv[i - 1];
  }
  argv[0] = name;

  argc++;

  return call_to_create_process(argc, argv, endless_loop, NULL);
}

int64_t my_nice(uint64_t pid, uint64_t new_prio) {
  return call_to_nice_process(pid, new_prio);
}

int64_t my_kill(uint64_t pid) { return call_to_pkill_process(pid); }

int64_t my_block(uint64_t pid) { return call_to_block_process(pid); }

int64_t my_unblock(uint64_t pid) { return call_to_unblock_process(pid); }

int64_t my_sem_open(char *sem_id, uint64_t initial_value) {
  return call_to_sem_open(initial_value, sem_id);
}

int64_t my_sem_wait(int sem_id) { return call_to_sem_wait(sem_id); }

int64_t my_sem_post(int sem_id) { return call_to_sem_post(sem_id); }

int64_t my_sem_close(char *sem_id) { return call_to_sem_close(sem_id); }

int64_t my_yield() {
  call_to_yield();
  return 1;
}

int64_t my_wait(int64_t pid) { return call_to_waitpid(pid); }
