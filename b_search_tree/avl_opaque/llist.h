/** @file llist.h
 *
 * @brief Library that supports both queue and stack handling for void*.
 */
#ifndef LLIST_H
#define LLIST_H

#include <stdint.h>

typedef struct llist_t llist_t;

/**
 * @brief Allocates a new linked list.
 *
 * @return A pointer to the newly created linked list, or NULL on failure.
 */
llist_t *llist_create();

/**
 * @brief Deletes a linked list and frees its memory.
 *
 * @param p_llist The linked list to delete.
 * @param destroy_data Function to delete individual data elements.
 */
void llist_delete(llist_t *p_llist, void (*destroy_data)(void *));

/**
 * @brief Adds a data element to the linked list as a stack.
 *
 * @param p_llist The linked list to add the data to.
 * @param data The data element to be added.
 * @return 1 on success, 0 on failure.
 */
int llist_push(llist_t *p_llist, void *data);

/**
 * @brief Removes a data element from the linked list as a stack.
 *
 * @param p_llist The linked list to remove the data from.
 * @return A pointer to the removed data element on success, or NULL on failure or empty list.
 */
void *llist_pop(llist_t *p_llist);

/**
 * @brief Adds a data element to the linked list as a queue.
 *
 * @param p_llist The linked list to add the data to.
 * @param data The data element to be added.
 * @return 1 on success, 0 on failure.
 */
int llist_enqueue(llist_t *p_llist, void *data);

/**
 * @brief Removes a data element from the linked list as a queue.
 *
 * @param p_llist The linked list to remove the data from.
 * @return A pointer to the removed data element on success, or NULL on failure or empty list.
 */
void *llist_dequeue(llist_t *p_llist);

/**
 * @brief Adds a data element before the specified target in the linked list.
 *
 * @param p_llist The linked list to add the data to.
 * @param target The data element before which the new data element should be added.
 * @param data The data element to be added.
 * @return 1 on success, 0 on failure.
 */
int llist_add_before(llist_t *p_llist, void *target, void *data);

/**
 * @brief Adds a data element after the specified target in the linked list.
 *
 * @param p_llist The linked list to add the data to.
 * @param target The data element after which the new data element should be added.
 * @param data The data element to be added.
 * @return 1 on success, 0 on failure.
 */
int llist_add_after(llist_t *p_llist, void *target, void *data);

/**
 * @brief Removes and returns the data element from the back of the linked list.
 *
 * @param p_llist The linked list from which to remove the data element.
 * @return A pointer to the removed data element on success, or NULL on failure or empty list.
 */
void *llist_extract_back(llist_t *p_llist);

/**
 * @brief Prints the nodes in the linked list.
 *
 * @param p_llist The linked list to print.
 * @param print_data Function to print individual data elements.
 */
void llist_print(llist_t *p_llist, void (*print_data)(void *));

/**
 * @brief Finds a specific data element in the linked list.
 *
 * @param p_llist The linked list to search in.
 * @param target The data element to search for.
 * @param compare_data Function to compare individual data elements.
 * @return A pointer to the found data element on success, or NULL if not found or empty list.
 */
void *llist_find(llist_t *p_llist, void *target, int (*compare_data)(void *initial, void *comparison));

/**
 * @brief Returns the size of the linked list.
 *
 * @param p_llist The linked list to get the size of.
 * @return The number of elements in the linked list.
 */
uint64_t get_size(llist_t *p_llist);

#endif /* LLIST_H */
