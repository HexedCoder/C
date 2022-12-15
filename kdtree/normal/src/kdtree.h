// ------------------------------------------------------------------------------
//Lesson 6 Exercise:binary search tree
// ------------------------------------------------------------------------------
#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stdio.h>
#include "pqueue.h"

struct tree {
	struct tree *parent;
	struct tree *left;
	struct tree *right;
	double x_coord;
	double y_coord;
	double distance;
};
typedef struct tree tree;

tree *create_tree_node(double, double);

void preorder(tree *root);

int find_median(FILE *input);

double get_distance(double x_val_1, double y_val_1, double x_val_2,
		    double y_val_2);

void postorder(tree *root);

void inorder(tree *root);

void level_order(tree *root);

int kd_insert(tree *root, tree *node, int method);

tree *search(tree *, double, double, int);

tree *nearest_neighbor(tree *root, double val_1, double val_2, double radius,
		       int method, pqueue_t *tree);

int bst_minimum(tree *tree);

tree *bst_minimum_node(tree *tree);

tree *bst_maximum_node(tree *tree);

int bst_maximum(tree *tree);

int tree_size(tree *tree);

void print(tree *);

tree *delete_node(tree **, int);

void tree_delete(tree **p_tree);

#endif
