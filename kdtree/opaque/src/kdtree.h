#ifndef KD_TREE_H
#define KD_TREE_H

#include <stdio.h>

typedef double (*compare)(void *, void *, int);

typedef void (*action)(void *);

typedef void (*destroy)(void *);

typedef struct tree tree;

typedef struct coordinate coordinate;

typedef struct coordinate {
	double x_coord;
	double y_coord;
} coordinate;

/**
 * @brief Function to allocate tree
 *
 * Provides user an address to a linked-list
 *
 * @return tree* On success, NULL on failure
 */
tree *tree_create(compare compare_func, action action_func,
		  destroy destroy_func);

int find_median(FILE * input);

void print_visual(tree * root);

void preorder(tree * root);

int get_income(tree * tree);

int get_age_income(tree * tree, int min, int max);

void postorder(tree * root);

void inorder(tree * root);

void levelorder(tree * root);

/**
 * @brief Adds void* data to linked-list as stack
 *
 * @param p_llist Linked-list which will hold data
 * @param data Void* being added to linked-list
 * @param method int to decide the method used to compare the nodes
 * @return 1 On success
 * @return 0 On failure
 */
int tree_insert(tree * root, void *data, int median);

void *tree_search(tree *, void *);

void *tree_minimum(tree * tree);

void *tree_maximum(tree * tree);

void *find_neighbor(double, double);

/**
 * @brief Checks size of linked-list
 *
 * @param p_llist Linked-list to check
 * @return uint64_t Number of elements in linked-list
 */
int tree_size(tree * tree);

int tree_delete(tree ** root, void *val);

/**
 * @brief Function to delete linked-list and free memory
 *
 * @param p_llist Linked-list to delete
 * @param destroy_data Function describing how to delete
 */
void tree_destroy(tree ** p_tree);

#endif				// KD_TREE_H
