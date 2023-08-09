/** @file avl.h
 *
 * @brief This module serves as the template for functions in avl.c
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
 */

#ifndef AVL_H
#define AVL_H

typedef struct avl_t avl_t;

// Defines the user's compare function
typedef int (*compare)(void *, void *);

// Defines the user's destroy function
typedef void (*destroy)(void *);

// Defines the user's action function
typedef void (*action)(void *, void *);

/**
 * Allocates memory for an avl_t tree.
 *
 * @param compare: Pointer to the comparison function.
 * @param action: Pointer to the action function.
 *
 * @return Pointer to the allocated avl_t tree on success, NULL on failure.
 */
avl_t *avl_tree_create(compare, action);

/**
 * Adds data of type void* to the AVL tree.
 *
 * @param avl_t *: Pointer to the root of the AVL tree.
 * @param void *: Pointer to the data being added to the tree.
 *
 * @return Pointer to the inserted node on success, NULL on failure.
 */
void *avl_insert(avl_t *, void *);

/**
 * Searches for data of type void* in the AVL tree.
 *
 * @param avl_t *: Pointer to the root of the AVL tree.
 * @param void *: Pointer to the data being searched for.
 *
 * @return Pointer to the node containing the data on success, NULL on failure.
 */
void *avl_tree_search(avl_t *, void *);

/**
 * Deletes the avl_t tree and frees the associated memory.
 *
 * @param avl_t **: Pointer to the pointer of the AVL tree to be deleted.
 * @param destroy: Pointer to the destroy function.
 */
void avl_tree_destroy(avl_t **, destroy);

/**
 * Returns the number of elements in the AVL tree.
 *
 * @param avl_t *: Pointer to the AVL tree.
 *
 * @return Number of elements in the AVL tree, or 0 on failure.
 */
int avl_tree_size(avl_t *);

int avl_node_delete(avl_t **root, void *val, destroy);

#endif /* AVL_H */

/* end of file */
