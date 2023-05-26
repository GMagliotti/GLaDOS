//  code to perform circular linked list operations
// https://www.programiz.com/dsa/circular-linked-list con nuevas funcionalidades.

#include "include/circularDoubleLinkedList.h"

struct Node* add_to_empty(struct Node* last, void * data) {
  if (last != NULL) return last;

  struct Node* newNode = (struct Node*)sys_malloc(sizeof(struct Node));

  newNode->data = data;
  last = newNode;

  last->next = last;
  last->prev = last;

  return last;
}

// add node to the front
struct Node* add_front(struct Node* last, void * data) {
  if (last == NULL) return NULL;

  struct Node* newNode = (struct Node*)sys_malloc(sizeof(struct Node));

  newNode->data = data;
  newNode->next = last->next;
  newNode->prev = last;
  last->next->prev = newNode;

  last->next = newNode;

  return last;
}

// add node to the end
struct Node* add_end(struct Node* last, void * data) {
  if (last == NULL)
    return add_to_empty(last, data);

  struct Node* newNode = (struct Node*)sys_malloc(sizeof(struct Node));
  if (newNode == NULL) {
      // Handle memory allocation failure
      // printf("Error: Failed to allocate memory for new node\n");
      return last;
  }

  newNode->data = data;
  newNode->next = last->next;
  newNode->prev = last;
  last->next->prev = newNode;

  last->next = newNode;
  last = newNode;

  return last;
}

struct Node* add_last(struct Node* last, void * data) {
  if (last == NULL) return NULL;

  if (last->next == NULL) return add_to_empty(last, data);

  struct Node* newNode = (struct Node*)sys_malloc(sizeof(struct Node));

  newNode->data = data;
  newNode->next = last;
  newNode->prev = last->prev;
  last->prev->next = newNode;

  last->prev = newNode;

  return last;
}

// insert node after a specific node
struct Node* add_after(struct Node* last, void * data, void * item) {
  // check if the list is empty
  if (last == NULL) return NULL;

  struct Node *newNode, *p;

  p = last->next;
  do {
  if (p->data == item) {
    newNode = (struct Node*)sys_malloc(sizeof(struct Node));

    newNode->data = data;
    newNode->next = p->next;
    newNode->prev = p;

    p->next->prev = newNode;
    p->next = newNode;

    if (p == last) last = newNode;
    return last;
  }

  p = p->next;
  } while (p != last->next);

  // printf("\nThe given node is not present in the list");
  return last;
}

// delete a node
void delete_node(struct Node** last, void * key) {
  if (*last == NULL) return;

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

  while (temp->next != *last && temp->next->data != key) temp = temp->next;

  if (temp->next->data == key) {
    d = temp->next;
    temp->next = d->next;
    temp->next->prev = temp;
    sys_free(d);
  }
}


void free_list(struct Node* last) {
if (last == NULL) {
        return;  // List is already empty
    }

    Node* current = last;

    while (current->next != current) {
      delete_node(&current, current->data);
    }
    delete_node(&current, current->data);

}


void traverse(struct Node* last) {
  struct Node* p;

  if (last == NULL) {
  // printf("The list is empty");
  return;
  }

  p = last->next;

  do {
  // printf("Imprimi algo");
  p = p->next;

  } while (p != last->next);
}
