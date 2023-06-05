// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/scheduler.h"
#include "include/process.h"
#include "include/roundRobin.h"

rr_queue_ptr rr_scheduler;

// creates a scheduler of the type RoundRobinWithPriority
rr_queue_ptr create_scheduler(void (*idle)(int, char **),
                              void (*shell)(int, char **)) {
  rr_scheduler = create_new_round_robin(initialize_idle(idle));
  char *argv[1] = {"Shell"};
  scheduler_create_process(1, argv, shell, NULL);
  next_process(rr_scheduler);

  return rr_scheduler;
}

process_ptr current_process(void) { return get_current_process(rr_scheduler); }

int get_current_pid(void) {
  process_ptr current = current_process();
  if (current == NULL)
    return ERROR;
  return current->pid;
}

bool initialized = false;

// function is called whenever a timertick occurs, or a process finishes
void *scheduler(void *rsp) {

  process_ptr current = get_current_process(rr_scheduler);

  if (initialized) {
    save_rsp(current, rsp);
  } else {
    initialized = true;
  }

  if (on_print_mode()) {
    print_current_process();
  }

  free_adopted_zombies(current->pid);

  scheduler_free_killed_children(current->pid);

  if (current->status == FINISHED) {
    current = finished_process_handler(current);

  } else if (current->status == BLOCKED || current->status == KILLED) {
    current = next_process(rr_scheduler);

  } else {
    current = next_tick(rr_scheduler);
  }

  set_current_process(current->pid);

  return (void *)context_switch(current);
}

// Adding a process to the scheduler
int scheduler_enqueue_process(process_ptr p) {
  if (enqueue_process(rr_scheduler, p) == NULL)
    return ERROR;
  return 1;
}

int scheduler_create_process(int argc, char **argv, void (*fn)(int, char **),
                             int fd[2]) {

  process_ptr created_process = create_process(argc, argv, fn, fd);
  if (created_process == NULL) {
    return ERROR;
  }
  if (scheduler_enqueue_process(created_process) == ERROR) {
    return ERROR;
  }

  return created_process->pid;
}

// Removing a process from the scheduler
int scheduler_dequeue_process(process_ptr p) {
  dequeue_process(rr_scheduler, p);
  return 1;
}

uint64_t context_switch(process_ptr process) { return process->rsp; }

void save_rsp(process_ptr process, uint64_t *to_save) {
  process->rsp = (uint64_t)to_save;
}

void free_scheduler(void) {
  uninitialize();
  free_round_robin(rr_scheduler);
}

int scheduler_waitpid(int pid) { return waitpid(pid); }

void scheduler_revive_process(int pid) {
  process_ptr proc = get_process(pid);
  if (proc != NULL && proc->status == BLOCKED) {
    proc->status = ALIVE;
  }
}

int scheduler_block_current_process() {
  int current_pid = get_current_pid();
  if (current_pid == ERROR) {
    return ERROR;
  }

  block_process(current_pid);
  return current_pid;
}

void print_scheduler_robin() { print_robin(rr_scheduler->list); }

void scheduler_free_killed_children(int pid) {
  process_ptr proc = get_process(pid);
  if (proc == NULL) {
    return;
  }
  for (int i = 0; i < proc->children_count; i++) {
    process_ptr child = get_process(proc->children[i]);
    if (child->status == KILLED) {
      scheduler_dequeue_process(get_process(proc->children[i]));
      free_process(proc->children[i]);
    }
  }
}

process_ptr finished_process_handler(process_ptr current) {
  process_ptr next = next_process(rr_scheduler);

  scheduler_dequeue_process(current);

  set_zombie(current->pid);

  return next;
}
