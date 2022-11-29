#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

typedef struct tree tree;

typedef int (*compare)(void *, void *);

typedef void (*action)(void *);

typedef struct person person;

typedef struct person {
	int age;
	int income;
	char *fname;
	char *lname;
	char *ssn;
} person;

/**
 * @brief Function to allocate tree
 *
 * Provides user an address to a linked-list
 *
 * @return tree* On success, NULL on failure
 */
tree *tree_create(compare compare_func, action action_func);

void print_visual(tree *root);

void preorder(tree *root);

int get_income(tree *tree);

int get_age_income(tree *tree, int min, int max);

void postorder(tree *root);

void inorder(tree *root);

void levelorder(tree *root);

/**
 * @brief Adds void* data to linked-list as stack
 *
 * @param p_llist Linked-list which will hold data
 * @param data Void* being added to linked-list
 * @return 1 On success
 * @return 0 On failure
 */
int tree_insert(tree *root, void *data);

void *tree_search(tree *, void *);

void *tree_minimum(tree *tree);

void *tree_maximum(tree *tree);

/**
 * @brief Checks size of linked-list
 *
 * @param p_llist Linked-list to check
 * @return uint64_t Number of elements in linked-list
 */
int tree_size(tree *tree);

int tree_delete(tree **root, void *val);

/**
 * @brief Function to delete linked-list and free memory
 *
 * @param p_llist Linked-list to delete
 * @param destroy_data Function describing how to delete
 */
void tree_destroy(tree **p_tree);

#endif
