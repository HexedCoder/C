/** @file pqueue.h
* @brief minimum priority queue (min-queue) library for void*
*/
#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdbool.h>
#include <stdint.h>

/**
* @brief User provided function to delete void* without memory leaks
*/
typedef void (*del_f)(void *data);

typedef struct pqueue_node {
	void *node_data;
	double priority;
} node_t;

struct pqueue_t {
	del_f delete;
	uint16_t count;
	uint16_t capacity;
	node_t *back;
	node_t *heap;
};

/**
* @brief Struct that defines pointer to minimum-node and
*        del_f that knows how to delete custom void*
*/
typedef struct pqueue_t pqueue_t;

/**
* @brief Creates a priority queue
*
* Provides user with the option of a custom delete function pending what
* datatype is stored within the queue.
*
* The purpose of this option, is so the library does not need to know
* anything about the user defined data structure to successfully delete
*
* @param capacity Maximum amount of elements that can be held in queue
* @param delete Function to delete void*. Pass NULL if not wanted
* @return pqueue_t* On success, NULL on failure
*/
pqueue_t *pqueue_create(uint16_t capacity, del_f delete);

/**
* @brief Deletes a priority queue, freeing resources used and
*        deleting any remaining items without warning
*
* @param pqueue The queue to delete
*/
void pqueue_delete(pqueue_t * pqueue);

/**
* @brief Inserts void* of a specified priority into min-queue
*
* @param pqueue Priority queue to query
* @param item Void* element to insert
* @param priority Item's priority (0-4,294,967,295) where 0 is top priority
*/
int pqueue_insert(pqueue_t * pqueue, void *item, double priority);

/**
* @brief Removes and returns the lowest-priority value from the queue
*
* @param pqueue Target priority queue
* @return Address of stored item
*/
void *pqueue_extract(pqueue_t * pqueue);

/**
* @brief Used to determine if pqueue_t is empty
*
* @param pqueue A priority queue
* @return True on empty, else False
*/
bool pqueue_is_empty(pqueue_t * pqueue);

/**
* @brief Used to determine if pqueue_t is full
*
* @param pqueue A priority queue
* @return True on full, else False
*/
bool pqueue_is_full(pqueue_t * pqueue);

void pqueue_print(pqueue_t * pqueue);

#endif				/* PQUEUE_H */
/*** END OF FILE ***/
