#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include <stdlib.h>

#define MAX_NAME 15
#define MAX_SEM 128

#define TRUE 1
#define FALSE 0

typedef struct pNode {
  struct pNode *next;
  uint64_t pid;
} sem_process_t;

typedef struct {
  char name[MAX_NAME];
  sem_process_t *first_process; // first process waiting in line
  sem_process_t *last_process;  // last process waiting in line
  uint64_t lock;
  int value;
  uint64_t size;      // amount of processes using the sem
  uint64_t size_list; // amount of blocked processes
} sem_t;

int find_available_semaphore();
uint64_t create_sem(uint64_t initial_value, char *name);
void init_sem();
void destroy_sem(int sem_index);
int sem_open(int initial_value, char *name);
int sem_close(char *name);
uint64_t sem_wait(uint64_t sem_idx);
uint64_t sem_post(uint64_t sem_idx);
int find_sem_index(char *name);
int sem_enqueue_process(int sem_index, int pid);
int sem_dequeue_process(int sem_index);

#endif