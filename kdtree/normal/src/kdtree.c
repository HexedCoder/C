#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "kdtree.h"
#include "llist.h"
#include "math.h"

typedef struct node_t {
	struct node_t *next;
	void *data;
} node_t;

tree *create_node(double val_1, double val_2)
{
	tree *tmp = NULL;

	tmp = calloc(1, sizeof(*tmp));
	if (tmp) {
		tmp->left = NULL;
		tmp->right = NULL;
		tmp->x_coord = val_1;
		tmp->y_coord = val_2;
		tmp->distance = DBL_MAX;
	}

	return tmp;
}				/* create_node() */

tree *bst_insert(tree * root, tree * node, int method)
{
	if (!root || !node) {
		return NULL;
	}

	while (root) {
		if (0 == method % 2) {
			if (node->x_coord < root->x_coord) {
				if (!root->left) {
					root->left = node;
					node->parent = root;
					break;
				} else {
					++method;
					root = root->left;
				}
			} else {
				if (!root->right) {
					root->right = node;
					node->parent = root;
					break;
				} else {
					++method;
					root = root->right;
				}
			}
		} else {
			if (node->y_coord < root->y_coord) {
				if (!root->left) {
					root->left = node;
					node->parent = root;
					break;
				} else {
					++method;
					root = root->left;
				}
			} else {
				if (!root->right) {
					root->right = node;
					node->parent = root;
					break;
				} else {
					++method;
					root = root->right;
				}
			}
		}
	}

	return root;
}

tree *search(tree * root, double val_1, double val_2, int method)
{
	if (!root) {
		return NULL;
	}

	if (0 == method % 2) {
		if (val_1 < root->x_coord) {
			return search(root->left, val_1, val_2, ++method);
		} else if (val_1 > root->x_coord) {
			return search(root->right, val_1, val_2, ++method);
		} else {
			return root;
		}
	} else {
		if (val_2 < root->y_coord) {
			return search(root->left, val_1, val_2, ++method);
		} else if (val_2 > root->y_coord) {
			return search(root->right, val_1, val_2, ++method);

		} else {
			return root;
		}
	}
}

tree *nearest_neighbor(tree * root, double val_1, double val_2, double radius,
		       int method, llist_t * list)
{
	if (!root) {
		return NULL;
	}

	double distance =
	    get_distance(root->x_coord, root->y_coord, val_1, val_2);
	root->distance = distance;

	if (distance <= radius) {
		printf("Neighbor (%lf, %lf) has distance %lf\n", root->x_coord,
		       root->y_coord, distance);
		//              llist_enqueue(list, root);
	} else {
		printf("Distance (%lf, %lf) too great from (%lf, %lf)\n", val_1,
		       val_2, root->x_coord, root->y_coord);
	}

	if (0 == method % 2) {
		if (val_1 < root->x_coord) {
			return nearest_neighbor(root->left, val_1, val_2,
						radius, ++method, list);
		} else if (val_1 > root->x_coord) {
			return nearest_neighbor(root->right, val_1, val_2,
						radius, ++method, list);
		} else {
			return root;
		}
	} else {
		if (val_2 < root->y_coord) {
			return nearest_neighbor(root->left, val_1, val_2,
						radius, ++method, list);
		} else if (val_2 > root->y_coord) {
			return nearest_neighbor(root->right, val_1, val_2,
						radius, ++method, list);

		} else {
			return root;
		}
	}
}

void preorder(tree * root)
{
	if (!root) {
		return;
	}

	printf("(%lf, %lf) ", root->x_coord, root->y_coord);
	preorder(root->left);
	preorder(root->right);
}

void postorder(tree * root)
{
	if (!root) {
		return;
	}

	postorder(root->left);
	postorder(root->right);
	printf("(%lf, %lf) ", root->x_coord, root->y_coord);
}

void inorder(tree * root)
{
	if (!root) {
		return;
	}

	inorder(root->left);
	printf("(%lf, %lf) ", root->x_coord, root->y_coord);
	inorder(root->right);
}

void level_order(tree * root)
{
	if (!root) {
		return;
	}

	llist_t *queue = llist_create();
	tree *curr = root;

	while (curr) {
		printf("(%lf, %lf) ", root->x_coord, root->y_coord);

		if (curr->left) {
			llist_enqueue(queue, curr->left);
		}
		if (curr->right) {
			llist_enqueue(queue, curr->right);
		}

		curr = llist_dequeue(queue);
	}
	llist_delete(queue, NULL);
}

int bst_minimum(tree * tree)
{
	while (tree->left)
		tree = tree->left;

	return tree->x_coord;
}

tree *bst_minimum_node(tree * tree)
{
	while (tree->left)
		tree = tree->left;

	return tree;
}

tree *bst_maximum_node(tree * tree)
{
	while (tree->right)
		tree = tree->right;

	return tree;
}

int bst_maximum(tree * tree)
{
	while (tree->right) {
		tree = tree->right;
	}

	return tree->x_coord;
}

int tree_size(tree * tree)
{
	if (!tree) {
		return 0;
	}
	int count = tree_size(tree->left);
	count += tree_size(tree->right);

	return 1 + count;
}

void print(tree * root)
{
	if (!root) {
		return;
	}

	print(root->left);
	printf("(%lf, %lf) ", root->x_coord, root->y_coord);
	print(root->right);
}

double get_distance(double x_val_1, double y_val_1, double x_val_2,
		    double y_val_2)
{
	double x_distance = (x_val_2 - x_val_1) * (x_val_2 - x_val_1);
	double y_distance = (y_val_2 - y_val_1) * (y_val_2 - y_val_1);

	return sqrt(x_distance + y_distance);
}

tree *delete_node(tree ** root, int val)
{
	val = (int)val;
	// Return parent of deleted node except:
	//
	// 1. if deleted node does not exist then return null
	// 2. if deleted node is the root then return new root
	tree *node = NULL;
	tree *tmp = 0;

	if (node == 0) {
		return *root;
	}
	// Root node.
	//
	// Treat special because root has no parent to relink.
	//
	// Only need special handling for:
	//
	// i. no children
	// ii. one child
	if (!node->parent) {
		if (!node->left && !node->right) {
			free(node);
			*root = 0;
			return *root;
		}
		if (!node->left || !node->right) {
			tmp = (!node->left) ? node->right : node->left;
			tmp->parent = 0;
			free(node);
			*root = tmp;
			return *root;
		}
	}
	// Replace deleted node with the minimum of its right branch.
	//
	// If missing the right branch then replace with the left branch, treating
	// leaf nodes in the same manner.
	//
	// The minimum node cannot have a left child.
	//
	// But the minimum node can be the right child of the deleted node so replace
	// deleted node's data with the minimum after testing if the minimum is its
	// right child.
	//
	// NOTE: If a leaf or missing right branch, then saving the node and copying
	// its data is extraneous but minimal work overall.
	tree *x = node;
	tmp = node->left;
	if (node->right != 0) {
		node = bst_minimum_node(node->right);
		tmp = node->right;
	}
	if (node->x_coord > node->parent->x_coord) {
		// Right child.
		node->parent->right = tmp;
	} else {
		// Left child.
		node->parent->left = tmp;
	}
	if (tmp != 0) {
		tmp->parent = node->parent;
	}
	x->x_coord = node->x_coord;

	tmp = node->parent;
	free(node);
	return tmp;
}

void tree_delete(tree ** p_tree)
{
	if (!p_tree || !*p_tree) {
		return;
	}

	tree_delete(&(*p_tree)->left);
	tree_delete(&(*p_tree)->right);

	free(*p_tree);
	*p_tree = NULL;
}

struct trunk {
	struct trunk *prev;
	const char *str;
};
