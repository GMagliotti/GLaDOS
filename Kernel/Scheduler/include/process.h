#ifndef PROCESS_H
#define PROCESS_H

#include "../../Semaphore/include/semaphore.h"
#include "../../include/lib.h"
#include "../../include/textDriver.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define ERROR -1

#define MAX_PROCESS_AMOUNT 128
#define STACK_SIZE 0x1000

// process status
#define ALIVE 0
#define READY 1
#define BLOCKED 2
#define ZOMBIE 3
#define FINISHED 4
#define KILLED 5

#define FOREGROUND 0
#define BACKGROUND 1

typedef struct registerBackup {
  uint64_t rax, rbx, rcx, rdx;
  uint64_t rsi, rdi, rsp, rbp;
  uint64_t r8, r9, r10, r11;
  uint64_t r12, r13, r14, r15;
  uint64_t rip, rflags, cs, ss;
} registerBackup;

typedef struct process {
  int pid;
  char *name;
  int ppid;     // parent pid
  int priority; // nice
  int current_lives;
  int status;
  int visibility; // foreground o background
  int og_visibility;
  int fd_r; // read
  int fd_w; // write
  int children[MAX_PROCESS_AMOUNT];
  int children_count;
  int ret_value;
  bool adopted;
  int done_sem;

  uint64_t rsp;
  uint64_t rbp;
} process;

typedef process *process_ptr;

process_ptr initialize_idle(void (*idle_fn)(int, char **));
void uninitialize(void);

process_ptr create_process(int argc, char **argv, void (*fn)(int, char **),
                           int fd[2]);
void ps(void);
int kill_process(int pid);
int kill_current_process();
int nice_process(int pid, int priority);
int block_process(int pid);
int unblock_process(int pid);
process_ptr get_process(int pid);
int get_foreground_process(void);
bool current_is_foreground();
void copy_args(char **destination, char **source, int amount);
void free_args(char **args, int argc);
int process_exists(int pid);
int get_free_pid(void);
char *get_process_status(int status);
void force_timer(void);

int free_process(int pid);
void save_children(int pid);
void free_shell();

void set_current_process(int new_pid);
void print_current_process();
extern void forceTimer(void);

bool wants_to_run(process_ptr process);
extern void initialize_stack(uint64_t new_program_stack, char **argv, int argc,
                             void (*fn)(int, char **),
                             void (*init)(int, char **,
                                          void (*fn)(int, char **)));

void init(int argc, char **argv, void (*fn)(int, char **));

void set_print_mode();
bool on_print_mode();

int waitpid(int pid);
void set_zombie(int pid);
void free_adopted_zombies(int pid);
void free_killed_children(int pid);

#endif
