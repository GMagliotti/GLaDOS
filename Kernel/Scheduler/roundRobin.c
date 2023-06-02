#include "include/roundRobin.h"

rr_queue_ptr create_new_round_robin(process_ptr idle) {
    if (idle == NULL) return NULL;
    //reservamos espacio para la estructura
    rr_queue * queue = sys_malloc( sizeof(rr_queue) );
    
    queue->list = add_to_empty(queue->list, idle);

    return queue;
}

int get_max_lives(int priority) {
    int lives = 1;
    if (priority < 0) lives = 0;

    for (int i = 0; i < priority; i++) {
        lives *= 2;
    } 

    return lives;
}

int is_sentinel(process_ptr process) {
    return process->priority == -1;
}

process_ptr find_next_process(rr_queue_ptr queue) {

    process_ptr start = queue->list->data;

    int needs_sentinel = 0;

    queue->list = queue->list->next;

    while (!wants_to_run(queue->list->data) && !needs_sentinel) {
        queue->list = queue->list->next;
        if (start == queue->list->data && !wants_to_run(queue->list->data)) {
            needs_sentinel = 1;
        }
    }

    if (needs_sentinel) {
        while (!is_sentinel(queue->list->data)) {
            queue->list = queue->list->next;
        }
    }

    return queue->list->data;
}

// Agregado de proceso al scheduler
NodeType enqueue_process(rr_queue_ptr queue, process * p) {
    if (p == NULL) return NULL;
    p->currentLives = get_max_lives(p->priority);
    return add_last(queue->list, (void *)p);
}


// Borrado de proceso al scheduler (lo deberiamos de poder llamar nosotros) que pasa cuando un proceso termina?
void dequeue_process(rr_queue_ptr queue, process * p) {
    delete_node(&queue->list, (void *)p); //borrará el nodo con pid: p->pid

    if (!wants_to_run(queue->list->data)) find_next_process(queue);

    return ;
}

bool has_next(NodeType current) {
    return current != NULL;
}

process_ptr next_process(rr_queue_ptr queue) {
    if (!has_next(queue->list)) return NULL;
    
    queue->list = queue->list->next;

    if (!wants_to_run(queue->list->data)) return find_next_process(queue);

    return queue->list->data;
}

process_ptr next_tick(rr_queue_ptr queue) {
    if (!has_next(queue->list)) return NULL;

    process_ptr process = (process_ptr)(queue->list->data);

    if (--process->currentLives == -1) {      // just in case me
        process->currentLives = get_max_lives(process->priority);
        return next_process(queue);
    } else if (process->currentLives < 0) {      // just in case me
        // printf("WAZAAAAAAAAA: Error. Un proceso tiene vidas negativas.");
    } else if (!process->currentLives) {
        process->currentLives = get_max_lives(process->priority);
        return next_process(queue);
    }

    return process;
}

process_ptr get_current_process(rr_queue_ptr queue) {
    if(queue->list == NULL)
        return NULL;
    return (process_ptr)(queue->list->data);
}


void free_round_robin(rr_queue_ptr queue) {
    free_list(queue->list);
    sys_free(queue);
}


// void printRobin(NodeType current) {
//     NodeType node = current; 
//     while (node->next != current) {
//         printf("%d (%i lives) - ", ((process *)(node->data))->pid, ((process *)(node->data))->currentLives);
//         node = node->next;
//     }
//     printf("%d (%i lives)\n", ((process *)(node->data))->pid, ((process *)(node->data))->currentLives);
// }

