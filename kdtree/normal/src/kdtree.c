#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "kdtree.h"
#include "math.h"

typedef struct kd_node_t {
	struct kd_node_t *next;
	void *data;
} kd_node_t;

tree *create_tree_node(double val_1, double val_2)
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
}				/* create_tree_node() */

int kd_insert(tree * root, tree * node, int method)
{
	if (!root || !node) {
		return 0;
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

	return 1;
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

int nearest_neighbor(tree * root, double val_1, double val_2, double radius,
		     int method, pqueue_t * list)
{
	if (!root) {
		return 0;
	}

	double distance =
	    get_distance(root->x_coord, root->y_coord, val_1, val_2);
	root->distance = distance;

	if (distance <= radius) {
		pqueue_insert(list, root, root->distance);
	}

	if (0 == method % 2) {
		if (val_1 < root->x_coord) {
			if (root->right) {
				distance = get_distance(root->right->x_coord,
							root->right->y_coord,
							val_1, val_2);

				if (distance <= radius) {
					root->right->distance = distance;
					pqueue_insert(list, root->right,
						      root->right->distance);
				}
			}

			return nearest_neighbor(root->left, val_1, val_2,
						radius, ++method, list);
		} else {
			if (root->left) {
				distance = get_distance(root->left->x_coord,
							root->left->y_coord,
							val_1, val_2);

				if (distance <= radius) {
					root->left->distance = distance;
					pqueue_insert(list, root->left,
						      root->left->distance);
				}
			}

			return nearest_neighbor(root->right, val_1, val_2,
						radius, ++method, list);
		}
	} else {
		if (val_2 < root->y_coord) {
			if (root->right) {
				distance = get_distance(root->right->x_coord,
							root->right->y_coord,
							val_1, val_2);

				if (distance <= radius) {
					root->right->distance = distance;
					pqueue_insert(list, root->right,
						      root->right->distance);
				}
			}

			return nearest_neighbor(root->left, val_1, val_2,
						radius, ++method, list);
		} else {
			if (root->left) {
				distance = get_distance(root->left->x_coord,
							root->left->y_coord,
							val_1, val_2);

				if (distance <= radius) {
					root->left->distance = distance;
					pqueue_insert(list, root->left,
						      root->left->distance);
				}
			}

			return nearest_neighbor(root->right, val_1, val_2,
						radius, ++method, list);
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
