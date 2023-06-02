#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct Node {
    uint64_t data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct DoubleLinkedList {
    Node* head;
    Node* tail;
    int size;
} DoubleLinkedListCDT;

#include "./include/doubleLinkedList.h"

DoubleLinkedListADT dll_init() {
    DoubleLinkedListADT dllist = (DoubleLinkedListADT) sys_malloc(sizeof(DoubleLinkedListCDT));
    dllist->head = NULL;
    dllist->tail = NULL;
    dllist->size = 0;
    return dllist;
}



Node * create_node(uint64_t data) {
    Node * ret_node = (Node *) sys_malloc(sizeof(Node));
    ret_node->data = data;
    ret_node->next = NULL;
    ret_node->prev = NULL;
    return ret_node;
}


void dll_insert_first(DoubleLinkedListADT list, uint64_t data) {
    if (list == NULL) return;
    Node * node = create_node(data);
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
    }
    (list->size)++;
}

void dll_insert_last(DoubleLinkedListADT list, uint64_t data) {
    if (list == NULL) return;
    Node * node = create_node(data);
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    (list->size)++;
}

Node * delete_node_rec(DoubleLinkedListADT list, Node * node, uint64_t data) {
    if (node == NULL) return NULL;
    else if (node->data == data) {
        if (node->next != NULL) {
            node->next->prev = node->prev;
        } else {
            list->tail = node->prev;
        }
        Node * backup = delete_node_rec(list, node->next, data);
        (list->size)--;
        sys_free(node);
        return backup;
    } else {
        node->next = delete_node_rec(list, node->next, data);
        return node;
    }
}

void dll_delete_all(DoubleLinkedListADT list, uint64_t data) {
    list->head = delete_node_rec(list, list->head, data);
}

// void dll_print(DoubleLinkedListADT list) {
//     Node * node = list->head;
//     for ( int i=0; i < list->size; i++ ) {
//         printf("%lu <-> ", node->data);
//         node = node->next;
//     }
//     printf("\n");
// }