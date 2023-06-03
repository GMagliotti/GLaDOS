#ifndef CIRCULAR_DOUBLE_LINKED_LIST_H
#define CIRCULAR_DOUBLE_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  void * data;
  struct Node* next;
  struct Node* prev;
} Node;

typedef Node * NodeType;

// funciones locales, que se podrían usar si se desea modificar la implementación
// struct Node* add_front(struct Node* last, void * data);
// struct Node* add_end(struct Node* last, void * data);
// struct Node* add_after(struct Node* last, void * data, void * item);

struct Node* add_to_empty(struct Node* last, void * data);
struct Node* add_last(struct Node* last, void * data);

void delete_node(struct Node** last, void * key);

void free_list(struct Node* last); 

// void traverse(struct Node* last);

#endif
