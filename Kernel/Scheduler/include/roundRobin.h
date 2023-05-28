#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include <stdbool.h>
#include <stdlib.h>
#include "circularDoubleLinkedList.h"
#include "process.h"

typedef struct round_robin_queue {
    NodeType list;
} rr_queue;

typedef rr_queue * rr_queue_ptr;

typedef process * process_ptr;

rr_queue_ptr create_new_round_robin(process_ptr idle);
NodeType enqueue_process(rr_queue_ptr queue, process *p);
void dequeue_process(rr_queue_ptr queue, process *p);
process_ptr next_process(rr_queue_ptr queue);
process_ptr next_tick(rr_queue_ptr queue);
process_ptr get_current_process(rr_queue_ptr queue);

void free_round_robin(rr_queue_ptr queue);

void printRobin(NodeType current);

#endif
