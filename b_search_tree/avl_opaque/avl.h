#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

typedef struct avl_t avl_t;

typedef int (*compare)(void *, void *);

typedef void (*action)(void *);

/**
 * @brief Function to allocate tree
 *
 * Provides user an address to a linked-list
 *
 * @return tree* On success, NULL on failure
 */
avl_t *tree_create(compare compare_func, action action_func);

void print_visual(avl_t * root);

void preorder(avl_t * root);

void postorder(avl_t * root);

void inorder(avl_t * root);

void level_order(avl_t * root);

/**
 * @brief Adds void* data to linked-list as stack
 *
 * @param p_llist Linked-list which will hold data
 * @param data Void* being added to linked-list
 * @return 1 On success
 * @return 0 On failure
 */
void *avl_insert(avl_t * root, void *data);

void *tree_search(avl_t *, void *);

void *tree_minimum(avl_t * tree);

void *tree_maximum(avl_t *tree);

/**
 * @brief Checks size of linked-list
 *
 * @param p_llist Linked-list to check
 * @return uint64_t Number of elements in linked-list
 */
int tree_size(avl_t *tree);

int tree_delete(avl_t **root, void *val);
int tree_pop(avl_t **root);

/**
 * @brief Function to delete linked-list and free memory
 *
 * @param p_llist Linked-list to delete
 * @param destroy_data Function describing how to delete
 */
void tree_destroy(avl_t **p_tree);

#endif
