// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//  code to perform circular linked list operations
// https://www.programiz.com/dsa/circular-linked-list con nuevas
// funcionalidades.

#include "include/circularDoubleLinkedList.h"

struct Node *add_to_empty(struct Node *last, void *data) {
  if (last != NULL)
    return last;

  struct Node *new_node = (struct Node *)sys_malloc(sizeof(struct Node));

  new_node->data = data;
  last = new_node;

  last->next = last;
  last->prev = last;

  return last;
}

struct Node *add_last(struct Node *last, void *data) {
  if (last == NULL)
    return NULL;

  if (last->next == NULL)
    return add_to_empty(last, data);

  struct Node *new_node = (struct Node *)sys_malloc(sizeof(struct Node));

  new_node->data = data;
  new_node->next = last;
  new_node->prev = last->prev;
  last->prev->next = new_node;

  last->prev = new_node;

  return last;
}

// delete a node
void delete_node(struct Node **last, void *key) {
  if (*last == NULL)
    return;

  if ((*last)->data == key && (*last)->next == *last) {
    sys_free(*last);
    *last = NULL;
    return;
  }

  struct Node *temp = *last, *d;

  if ((*last)->data == key) {
    temp = temp->prev;

    temp->next = (*last)->next;
    temp->next->prev = temp;
    sys_free(*last);
    *last = temp->next;
  }

  while (temp->next != *last && temp->next->data != key)
    temp = temp->next;

  if (temp->next->data == key) {
    d = temp->next;
    temp->next = d->next;
    temp->next->prev = temp;
    sys_free(d);
  }
}

void free_list(struct Node *last) {
  if (last == NULL) {
    return; // List is already empty
  }

  Node *current = last;

  while (current->next != current) {
    delete_node(&current, current->data);
  }
  delete_node(&current, current->data);
}

