#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include <stdlib.h>

#define MAX_NAME 15
#define MAX_SEM 30

#define TRUE 1
#define FALSE 0

typedef struct pNode {
  struct pNode *next;
  uint64_t pid;
} sem_process_t;

typedef struct {
  char name[MAX_NAME];
  sem_process_t *first_process; // primer proceso esperando (waiting) en la fila
  sem_process_t *last_process;  // ultimo proceso esperando en la fila
  uint64_t lock;
  int value;
  uint64_t size;      // cantidad de procesos que usan el sem
  uint64_t size_list; // cantidad de procesos bloqueados
} sem_t;

int find_available_semaphore();
uint64_t create_sem(uint64_t initial_value, char *name);
void init_sem();
void destroy_sem(int sem_index);
int sem_open(char *name);
int sem_close(char *name);
// Locks a semaphore. If successful (the lock was acquired), sem_wait() and
// sem_trywait() will return 0.  Otherwise, -1 is returned and errno is set, and
// the state of the semaphore is unchanged.
uint64_t sem_wait(uint64_t sem_idx);
// the value of the semaphore is incremented, and all threads which are waiting
// on the semaphore are awakened. If successful, sem_post() will return 0.
// Otherwise, -1 is returned.
uint64_t sem_post(uint64_t sem_idx);
// void sem();
int find_sem_index(char *name);
// char *getSemName(uint64_t semIndex);
// void printProcessesSem(uint64_t semIndex);
int sem_enqueue_process(int sem_index, int pid);
int sem_dequeue_process(int sem_index);

#endif