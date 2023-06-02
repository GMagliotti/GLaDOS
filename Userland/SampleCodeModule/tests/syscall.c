#include "syscall.h"

int64_t my_getpid() {
  return call_to_getpid();
}

int64_t my_create_process(char *name, uint64_t argc, char *argv[]) {

  for (int i = argc; i > 0; i--) {
    argv[i] = argv[i - 1];
  }
  argv[0] = name;

  return call_to_create_process(argc, argv, endless_loop);
}

int64_t my_nice(uint64_t pid, uint64_t newPrio) {
  return call_to_nice_process(pid, newPrio);
}

int64_t my_kill(uint64_t pid) {
  return call_to_pkill_process(pid);
}

int64_t my_block(uint64_t pid) {
  return call_to_block_process(pid);
}

int64_t my_unblock(uint64_t pid) {
  return 0;
}

int64_t my_sem_open(char *sem_id, uint64_t initialValue) {
  return 0;
}

int64_t my_sem_wait(char *sem_id) {
  return 0;
}

int64_t my_sem_post(char *sem_id) {
  return 0;
}

int64_t my_sem_close(char *sem_id) {
  return 0;
}

int64_t my_yield() {
  return 0;
}

int64_t my_wait(int64_t pid) {
  return 0;
}
