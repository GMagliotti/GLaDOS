#include "include/roundRobin.h"

rr_queue_ptr create_new_round_robin(process_ptr idle) {
  if (idle == NULL)
    return NULL;
  // reservamos espacio para la estructura
  rr_queue *queue = sys_malloc(sizeof(rr_queue));

  queue->list = add_to_empty(queue->list, idle);

  return queue;
}

int get_max_lives(int priority) {
  int lives = 1;
  if (priority < 0)
    lives = 0;

  for (int i = 0; i < priority; i++) {
    lives *= 2;
  }

  return lives;
}

int is_sentinel(process_ptr process) { return process->priority == -1; }

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
NodeType enqueue_process(rr_queue_ptr queue, process *p) {
  if (p == NULL)
    return NULL;
  p->current_lives = get_max_lives(p->priority);
  return add_last(queue->list, (void *)p);
}

// Borrado de proceso al scheduler (lo deberiamos de poder llamar nosotros) que
// pasa cuando un proceso termina?
void dequeue_process(rr_queue_ptr queue, process *p) {
  delete_node(&queue->list, (void *)p); // borrará el nodo con pid: p->pid

  if (!wants_to_run(queue->list->data))
    find_next_process(queue);

  return;
}

bool has_next(NodeType current) { return current != NULL; }

process_ptr next_process(rr_queue_ptr queue) {
  if (!has_next(queue->list))
    return NULL;

  queue->list = queue->list->next;

  if (!wants_to_run(queue->list->data))
    return find_next_process(queue);

  return queue->list->data;
}

process_ptr next_tick(rr_queue_ptr queue) {
  if (!has_next(queue->list))
    return NULL;

  process_ptr process = (process_ptr)(queue->list->data);

  if (--process->current_lives == -1) {
    process->current_lives = get_max_lives(process->priority);
    return next_process(queue);
  } else if (process->current_lives < 0) { // error case
    // print_string("Error: Un proceso tiene vidas negativas.", 64);
    return NULL;
  } else if (!process->current_lives) {
    process->current_lives = get_max_lives(process->priority);
    return next_process(queue);
  }

  return process;
}

process_ptr get_current_process(rr_queue_ptr queue) {
  if (queue->list == NULL)
    return NULL;
  return (process_ptr)(queue->list->data);
}

void free_round_robin(rr_queue_ptr queue) {
  free_list(queue->list);
  sys_free(queue);
}

void print_robin(NodeType current) {
  NodeType node = current;
  while (node->next != current) {
    print_number(((process *)(node->data))->pid, 10);
    print_string(" (", 4);
    print_number(((process *)(node->data))->current_lives, 10);
    print_string(" lives) - ", 15);
    node = node->next;
  }
  print_number(((process *)(node->data))->pid, 10);
  print_string(" (", 4);
  print_number(((process *)(node->data))->current_lives, 10);
  print_string(" lives)\n", 15);
  node = node->next;
}
