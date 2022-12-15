/** @file pqueue.h
* @brief min-queue library for to implement pqueue.c
*/
#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdbool.h>

/**
* @brief User-defined delete function
*/
typedef void (*delete_func)(void *data);
typedef struct pqueue_t pqueue_t;

typedef struct pqueue_node {
	void *node_data;
	double priority;
} node_t;

/**
* @brief Creates a priority queue
*
* Provides interface for user to define their own free functions
*
* @param capacity Maximum size of queue
* @param delete Function to define proper memory free
* @return pqueue_t* On success, NULL on failure
*/
pqueue_t *pqueue_create(int capacity, delete_func delete);

/**
* @brief Destroys a priority queue
*
* @param pqueue pqueue to be deleted
*/
void pqueue_destroy(pqueue_t * pqueue);

/**
* @brief Inserts void* of a specified priority into min-queue
*
* @param pqueue Priority queue insert item into
* @param item Void* element to insert
* @param priority the weight of the item being entered
*/
int pqueue_insert(pqueue_t * pqueue, void *item, double priority);

/**
* @brief Removes and returns the lowest value from pqueue
*
* @param pqueue pqueue to extract form
* @return Address of stored item or NULL on empty pqueue
*/
void *pqueue_extract(pqueue_t * pqueue);

/**
* @brief Used to determine if pqueue_t is empty
*
* @param pqueue priority queue to check for size
* @return 1 on empty, else 0
*/
int pqueue_is_empty(pqueue_t * pqueue);

/**
* @brief Used to determine if pqueue_t is full
*
* @param pqueue priority queue to check for size
* @return True on full, else False
*/
bool pqueue_is_full(pqueue_t * pqueue);

/**
* @brief Used to print elements in pqueue
*
* @param pqueue priority queue to print first value
*/
void pqueue_print(pqueue_t * pqueue);

#endif				/* PQUEUE_H */
/*** END OF FILE ***/
