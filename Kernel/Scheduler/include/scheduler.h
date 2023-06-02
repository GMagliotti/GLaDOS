#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "roundRobin.h"
#include <MemoryManager.h>

rr_queue_ptr create_scheduler(void (*idle)(int, char **), void (*shell)(int, char **));
process_ptr current_process(void);
void * scheduler(void * rsp);

int scheduler_enqueue_process(process_ptr p);
int scheduler_create_process(int argc, char** argv, void (*fn)(int, char **));
int scheduler_dequeue_process(process_ptr p);


uint64_t context_switch(process_ptr process);
void save_rsp(process_ptr process, uint64_t * to_save);

void free_scheduler(void);
int get_current_pid(void);

void scheduler_revive_process(int pid);
int scheduler_block_current_process();

#endif
