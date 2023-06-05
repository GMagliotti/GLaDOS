// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/scheduler.h"
#include "include/process.h"
#include "include/roundRobin.h"

rr_queue_ptr rr_scheduler;

// newScheduler: crea un scheduler de tipo RoundRobinWithPriority
rr_queue_ptr create_scheduler(void (*idle)(int, char **),
                              void (*shell)(int, char **)) {
  rr_scheduler = create_new_round_robin(initialize_idle(idle));
  char *argv[1] = {"Shell"};
  scheduler_create_process(1, argv, shell, NULL);
  next_process(rr_scheduler);

  return rr_scheduler;
}

// para que se pueda acceder al current_process estando en process
process_ptr current_process(void) { return get_current_process(rr_scheduler); }

int get_current_pid(void) {
  process_ptr current_proc = current_process();
  if (current_proc == NULL)
    return ERROR;
  return current_proc->pid;
}

bool initialized = false;

// función que se llama cada vez que ocurre el timertick, o termina un proceso.
void *scheduler(void *rsp) {

  process_ptr current_process = get_current_process(rr_scheduler);

  if (initialized) {
    save_rsp(current_process, rsp);
  } else {
    initialized = true;
  }

  if (on_print_mode()) {
    print_current_process();
  }

  free_adopted_zombies(current_process->pid);

  scheduler_free_killed_children(current_process->pid);

  if (current_process->status == FINISHED) {
    current_process = finished_process_handler(current_process);

  } else if (current_process->status == BLOCKED ||
             current_process->status == KILLED) {
    current_process = next_process(rr_scheduler);

  } else {
    current_process = next_tick(rr_scheduler);
  }

  set_current_process(current_process->pid);

  return (void *)context_switch(current_process);
}

// Agregado de proceso al scheduler
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

// Borrado de proceso al scheduler (lo deberiamos de poder llamar nosotros) ¿que
// pasa cuando un proceso termina?
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
    // proc->visibility = proc->og_visibility;
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

process_ptr finished_process_handler(process_ptr current_process) {
  process_ptr next = next_process(rr_scheduler);

  scheduler_dequeue_process(current_process);

  set_zombie(current_process->pid);

  return next;
}

// #include <stdio.h>
// #include <string.h>

// int main(int argc, char * argv[]) {

//     uint64_t the_process = 0;

//     rr_scheduler = create_scheduler();

//     char * procNames[20] = {
//         "Sneezy Boogle",
//         "Giggles McWiggle",
//         "Wobble Doodle",
//         "Snickerdoodle",
//         "Bumble Whimsy",
//         "Ziggy Zoodle",
//         "Flibbertigibbet",
//         "Quibble Quackington",
//         "Wobblefluff",
//         "Jellybean McSquish",
//         "Noodle Nutter",
//         "Guffaw Snickerbottom",
//         "Whimsical Wobblebottom",
//         "Chuckletwist",
//         "Doodlebug McSillypants",
//         "Gigglefritz",
//         "Fizzbang McGiggle",
//         "Mirthful McSprinkle",
//         "Teehee Wobblekins",
//         "Guffaw Snickerdoodle" };

//     int fd[2] = { 10, 10};

//     char input[100];

//     int count = 0;

//     while (1) {
//         process_ptr proc = (process_ptr)(rr_scheduler->list->data);

//         printf("TheProcess: %lx\n", the_process);

//         printf("\nRRLIST\n");
//         print_robin(rr_scheduler->list);
//         printf("-- Por ejecutar: %s - Process status: %s, RSP: %lx, Remaining
//         lives: %i --\n", proc->name, getProcessStatus((int)proc->status),
//         the_process, proc->current_lives); printf("-- Tiene %i hijos: ",
//         proc->children_count);

//         for (int i = 0; i< proc->children_count; i++) {
//             process_ptr child = get_process(proc->children[i]);
//             printf("%i - %s, ", child->pid, child->name);
//         }

//         printf("\nEnter an operation [ 'next', 'kill', 'block', 'finished',
//         'add', 'print', 'end' ]: "); fgets(input, sizeof(input), stdin);
//         input[strcspn(input, "\n")] = '\0';

//         if (strcmp(input, "end") == 0 || strcmp(input, "e") == 0) {
//             break;  // Exit the loop if "end" is entered
//         } else if ( strcmp(input, "next") == 0 || strcmp(input, "n") == 0 ||
//         strcmp(input, "") == 0 ) {

//         } else if (strcmp(input, "kill") == 0 || strcmp(input, "k") == 0) {
//             if (proc->priority != -1 && proc->pid != 0) {
//                 kill_process(proc->pid);
//             }
//         } else if (strcmp(input, "block") == 0 || strcmp(input, "b") == 0) {
//             if (proc->priority != -1 && proc->pid != 0) {
//                 proc->status = BLOCKED;
//             }
//         } else if (strcmp(input, "finished") == 0 || strcmp(input, "f") == 0)
//         {
//             if (proc->priority != -1 && proc->pid != 0) {
//                 proc->status = FINISHED;
//             }
//         } else if (strcmp(input, "add") == 0 || strcmp(input, "a") == 0) {
//             scheduler_create_process(procNames[count++ % 20], 0, NULL, NULL,
//             FOREGROUND, fd);
//         } else if (strcmp(input, "print") == 0 || strcmp(input, "p") == 0) {
//             ps();
//         }

//         uint64_t rsp = proc->rsp;

//         the_process = *(uint64_t *)(scheduler(&rsp));

//     }

//     free_scheduler();

//     return 0;
// }
