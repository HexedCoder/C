#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

typedef struct avl_t avl_t;

// Defines the user's compare function
typedef int (*compare)(void *, void *);

// Defines the user's destroy function
typedef void (*destroy)(void *);

// Defines the user's action function
typedef void (*action)(void *);

/**
 * Allocates memory for an avl_t tree.
 *
 * @param compare_func Pointer to the comparison function.
 * @param action_func Pointer to the action function.
 * @return Pointer to the allocated avl_t tree on success, NULL on failure.
 */
avl_t *tree_create(compare compare_func, action action_func);

/**
 * Prints the AVL tree in a visual format.
 *
 * @param p_root Pointer to the p_root of the AVL tree.
 */
void print_visual(avl_t *p_root);

/**
 * Performs the preorder action (provided on tree creation) on the tree.
 *
 * @param p_tree Pointer to the p_tree of the AVL tree.
 */
void preorder(avl_t *p_tree);

/**
 * Performs the postorder action (provided on tree creation) on the tree.
 *
 * @param p_tree Pointer to the p_tree of the AVL tree.
 */
void postorder(avl_t *p_tree);

/**
 * Performs the inorder action (provided on tree creation) on the tree.
 *
 * @param p_tree Pointer to the p_tree of the AVL tree.
 */
void inorder(avl_t *p_tree);

/**
 * Performs the level order action (provided on tree creation) on the tree.
 *
 * @param p_tree Pointer to the p_tree of the AVL tree.
 */
void level_order(avl_t *p_tree);

/**
 * Adds p_data of type void* to the AVL tree.
 *
 * @param p_tree Pointer to the p_tree of the AVL tree.
 * @param p_data Pointer to the p_data being added to the tree.
 * @return Pointer to the inserted node on success, NULL on failure.
 */
void *avl_insert(avl_t *p_tree, void *p_data);

/**
 * Searches for p_value of type void* in the AVL tree.
 *
 * @param p_tree Pointer to the p_tree of the AVL tree.
 * @param p_value Pointer to the p_value being searched for.
 * @return Pointer to the node containing the p_value on success, NULL on
 * failure.
 */
void *tree_search(avl_t *p_tree, void *p_value);

/**
 * Returns the minimum node in the AVL p_tree.
 *
 * @param p_tree Pointer to the AVL p_tree.
 * @return Pointer to the minimum node on success, NULL on failure.
 */
void *tree_minimum(avl_t *p_tree);

/**
 * Returns the maximum node in the AVL p_tree.
 *
 * @param p_tree Pointer to the AVL p_tree.
 * @return Pointer to the maximum node on success, NULL on failure.
 */
void *tree_maximum(avl_t *p_tree);

/**
 * Returns the number of elements in the AVL p_tree.
 *
 * @param p_tree Pointer to the AVL p_tree.
 * @return Number of elements in the AVL p_tree.
 */
int tree_size(avl_t *p_tree);

/**
 * Deletes the avl_t tree and frees the associated memory.
 *
 * @param pp_tree Pointer to the pointer of the AVL tree to be deleted.
 */
void tree_destroy(avl_t **pp_tree);

int tree_delete(avl_t **pp_root, void *p_data);

#endif
