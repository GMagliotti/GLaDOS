#ifndef _DOUBLELINKEDLIST_H
#define _DOUBLELINKEDLIST_H

#include <syscall.h>

typedef DoubleLinkedListCDT * DoubleLinkedListADT;

/**
 * @brief Initializes a doubly linked list
 * 
 * @return DoubleLinkedListADT The address to the initialized doubly linked list
 */
DoubleLinkedListADT dll_init();

/**
 * @brief Inserts a node at the beginning of a doubly linked list
 * 
 * @param list An initialized doubly linked list
 * @param data The data that the new node will contain
 */
void dll_insert_first(DoubleLinkedListADT list, uint64_t data);

/**
 * @brief Inserts a node at the end of a doubly linked list
 * 
 * @param list An initialized doubly linked list
 * @param data The data that the new node will contain
 */
void dll_insert_last(DoubleLinkedListADT list, uint64_t data);

/**
 * @brief Deletes all nodes that contain the data provided in a doubly linked list
 * 
 * @param list An initialized doubly linked list
 * @param data The data to match to eliminate nodes
 */
void dll_delete_all(DoubleLinkedListADT list, uint64_t data);

/**
 * @brief Prints the contents of all nodes, from head to tail, in order
 * 
 * @param list An initialized doubly linked list
 */
void dll_print(DoubleLinkedListADT list);

#endif
