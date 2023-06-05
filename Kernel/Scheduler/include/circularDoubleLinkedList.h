#ifndef CIRCULAR_DOUBLE_LINKED_LIST_H
#define CIRCULAR_DOUBLE_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  void *data;
  struct Node *next;
  struct Node *prev;
} Node;

typedef Node *NodeType;

struct Node *add_to_empty(struct Node *last, void *data);
struct Node *add_last(struct Node *last, void *data);

void delete_node(struct Node **last, void *key);

void free_list(struct Node *last);

extern void sys_free(void *memptr);
extern void *sys_malloc(size_t requested_memory);


#endif
