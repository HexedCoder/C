/** @file llist.h
*
* @brief library which supports both queue and stack handling for void*
*/
#ifndef LLIST_H
#define LLIST_H

#include <stdint.h>

typedef struct llist_t llist_t;

/**
 * @brief Function to allocate linked-list
 *
 * Provides user an address to a linked-list
 *
 * @return llist_t* On success, NULL on failure
 */
llist_t *llist_create();

/**
 * @brief Function to delete linked-list and free memory
 *
 * @param p_llist Linked-list to delete
 * @param destroy_data Function describing how to delete
 */
void llist_delete(llist_t * p_llist, void (*destroy_data)(void *));

/**
 * @brief Adds void* data to linked-list as stack
 *
 * @param p_llist Linked-list which will hold data
 * @param data Void* being added to linked-list
 * @return 1 On success
 * @return 0 On failure
 */
int llist_push(llist_t * p_llist, void *data);

/**
 * @brief Removes void* data from linked-list as stack
 *
 * @param p_llist Linked-list holding nodes
 * @return void* On success
 * @return NULL On failure
 */
void *llist_pop(llist_t * p_llist);

/**
 * @brief Adds void* data to linked-list as queue
 *
 * @param p_llist Linked-list which will hold data
 * @param data Void* being added to linked-list
 * @return 1 On success
 * @return 0 On failure
 */
int llist_enqueue(llist_t * p_llist, void *data);

/**
 * @brief Removes void* data from linked-list as queue
 *
 * @param p_llist Linked-list holding nodes
 * @return void* On success
 * @return NULL On failure
 */
void *llist_dequeue(llist_t * p_llist);

int llist_add_before(llist_t * p_llist, void *target, void *data);

int llist_add_after(llist_t * p_llist, void *target, void *data);

void *llist_extract_back(llist_t * p_llist);

/**
 * @brief Function to print nodes
 *
 * @param p_llist Linked-list to print
 * @param print_data Function describing how to print nodes
 */
void llist_print(llist_t * p_llist, void (*print_data)(void *));

/**
 * @brief Function to find specific node
 *
 * @param p_llist Linked-list to check
 * @param target void* Value to search for with compare_data
 * @param compare_data Function describing how to compare nodes
 */
void *llist_find(llist_t *p_llist, void *target,
		 int (*compare_data)(void *initial, void *comparison));

/**
 * @brief Checks size of linked-list
 *
 * @param p_llist Linked-list to check
 * @return uint64_t Number of elements in linked-list
 */
uint64_t get_size(llist_t *p_llist);

#endif				/* LLIST_H */

/*** end of file ***/
