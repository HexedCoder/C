#include <stdlib.h>
#include <stdio.h>
#include <stdint-gcc.h>

#include "avl.h"
#include "llist.h"

typedef struct node_t {
	struct node_t *parent;
	struct node_t *left;
	struct node_t *right;
	int height;
	void *data;
} node_t;

struct tree {
	struct node_t *root;
	compare compare_func;
	action action_func;
};

static node_t *create_node(void *data);

static void preorder_action(node_t *node, action action_func);

static void postorder_action(node_t *node, action action_func);

static void inorder_action(node_t *node, action action_func);

static void *get_max(node_t *node);

static void *get_min(node_t *node);

static int get_t_size(node_t *node);

tree *tree_create(compare compare_func, action action_func)
{
	tree *tree = calloc(1, sizeof(*tree));

	tree->compare_func = compare_func;
	tree->action_func = action_func;

	return tree;
} /* tree_create() */

static node_t *create_node(void *data)
{
	node_t *node = NULL;

	node = calloc(1, sizeof(*node));
	if (node) {
		node->left = NULL;
		node->right = NULL;
		node->parent = NULL;
		node->height = 0;
		node->data = data;
	}

	return node;
} /* create_node() */

static int get_t_size(node_t *node)
{
	if (!node) {
		return 0;
	}

	int count = get_t_size(node->left);
	count += get_t_size(node->right);

	return 1 + count;
}

static int insert(tree *tree, void *data)
{
	int ret = 0;

	if (!tree || !data) {
		goto INSERT_EXIT;
	}

	if (!tree->compare_func) {
		printf("Null compare function given\n");
		goto INSERT_EXIT;
	}

	if (!tree->root) {
		node_t *new_node = create_node(data);

		tree->root = new_node;
		ret = 1;
		goto INSERT_EXIT;
	}

	node_t *node = tree->root;

	while (node) {
		ret = tree->compare_func(node->data, data);

		if (0 > ret) {
			if (!node->left) {
				node_t *new_node = create_node(data);

				if (!new_node) {
					goto INSERT_EXIT;
				}

				node->left = new_node;
				new_node->parent = node;
				break;
			} else {
				node = node->left;
			}
		} else if (0 < ret) {
			if (!node->right) {
				node_t *new_node = create_node(data);

				if (!new_node) {
					goto INSERT_EXIT;
				}

				node->right = new_node;
				new_node->parent = node;
				break;
			} else {
				node = node->right;
			}
		} else {
			break;
		}
	}

	node->height = 1;
	//	int bf = 0;

	//	// Update heights and test for AVL violation.
	//	while (node->parent != 0 && bf <= 1) {
	//		node = node->parent;
	//		int lh = (node->left) ? node->left->height : 0;
	//		int rh = (node->right) ? node->right->height : 0;
	//		int max = (rh > lh) ? rh : lh;
	//		bf = abs(rh - lh);
	//		node->height = max + 1;
	//	}
	//	if (node->parent == 0 || bf <= 1) {
	//		return 0;
	//	}
	//
	//	ret = tree->compare_func(data < node->data);
	//
	//	// Automatic correction of AVL violation (self-balance).
	//	if (-1 == ret) {
	//		// Determine if grandchild of first imbalanced node is LL or LR.
	//		ret = tree->compare_func(data, node->left->data);
	//
	//		if (-1 == ret) {
	//			// LL
	//			node = rotate_right(&root, node->left);
	//			node->right->height = node->height - 1;
	//		} else {
	//			// LR
	//			node = rotate_right(
	//				&root, rotate_left(&root, node->left->right));
	//			if (node != 0) {
	//				node->left->height = node->height;
	//				node->right->height = node->height;
	//				++(node->height);
	//			}
	//		}
	//	} else {
	//		// Determine if grandchild of first imbalanced node is RR or RL.
	//		if (data->data > node->right->data) {
	//			// RR
	//			node = rotate_left(&root, node->right);
	//			node->left->height = node->height - 1;
	//		} else {
	//			// RL
	//			node = rotate_left(
	//				&root, rotate_right(&root, node->right->left));
	//			if (node != 0) {
	//				node->left->height = node->height;
	//				node->right->height = node->height;
	//				++(node->height);
	//			}
	//		}
	//	}

	ret = 1;

INSERT_EXIT:
	return ret;
} /* tree_insert() */

//tree *rotate_right(tree **root, tree *new_node)
//{
//	// Return if node is null or the root, or is not a left child.
//	if (new_node == 0 || new_node == *root ||
//	    new_node->data > new_node->parent->data)
//		return new_node;
//	tree *parent = new_node->parent;
//	tree *parent_parent = parent->parent;
//	tree *child = new_node->right;
//	new_node->parent = parent_parent;
//	new_node->right = parent;
//	parent->parent = new_node;
//	parent->left = child;
//	if (child != 0) {
//		child->parent = parent;
//	}
//	if (parent_parent != 0) {
//		if (parent->data < parent_parent->data) {
//			parent_parent->left = new_node;
//		} else {
//			parent_parent->right = new_node;
//		}
//	} else {
//		// Parent of rotated node is the root; rotated node becomes the new root.
//		*root = new_node;
//	}
//	return new_node;
//}
//
//tree *rotate_left(tree **t, tree *n)
//{
//	// Return if node is null or the root, or is not a right child.
//	if (n == 0 || n == *t || n->data < n->parent->data)
//		return n;
//	tree *p = n->parent;
//	tree *pp = p->parent;
//	tree *c = n->left;
//	n->parent = pp;
//	n->left = p;
//	p->parent = n;
//	p->right = c;
//	if (c != 0) {
//		c->parent = p;
//	}
//	if (pp != 0) {
//		if (p->data < pp->data) {
//			pp->left = n;
//		} else {
//			pp->right = n;
//		}
//	} else {
//		// Parent of rotated node is the root; rotated node becomes the new root.
//		*t = n;
//	}
//	return n;
//}

int avl_insert(tree *root, void *data)
{
	// Regular binary search tree insertion.
	return insert(root, data);
}

static void *find(node_t *node, void *value, compare compare_func)
{
	if (!node) {
		return NULL;
	}

	int ret = compare_func(value, node->data);

	if (ret > 0) {
		return find(node->left, value, compare_func);
	} else if (ret < 0) {
		return find(node->right, value, compare_func);
	} else {
		return node;
	}
}

void *tree_search(tree *tree, void *value)
{
	if (!tree || !value) {
		return NULL;
	}

	return find(tree->root, value, tree->compare_func);
}

static void preorder_action(node_t *node, action action_func)
{
	if (!node) {
		return;
	}

	action_func(node->data);
	preorder_action(node->left, action_func);
	preorder_action(node->right, action_func);
}

static void postorder_action(node_t *node, action action_func)
{
	if (!node) {
		return;
	}

	postorder_action(node->left, action_func);
	postorder_action(node->right, action_func);
	action_func(node->data);
}

static void inorder_action(node_t *node, action action_func)
{
	if (!node) {
		return;
	}

	inorder_action(node->left, action_func);
	action_func(node->data);
	inorder_action(node->right, action_func);
}

void preorder(tree *tree)
{
	if (!tree) {
		return;
	}

	if (!tree->action_func) {
		printf("Invalid action function provided\n");
		return;
	}

	preorder_action(tree->root, tree->action_func);
	printf("\n");
}

void postorder(tree *tree)
{
	if (!tree) {
		return;
	}

	if (!tree->action_func) {
		printf("Invalid action function provided\n");
		return;
	}

	postorder_action(tree->root, tree->action_func);
	printf("\n");
}

void inorder(tree *tree)
{
	if (!tree) {
		return;
	}

	if (!tree->action_func) {
		printf("Invalid action function provided\n");
		return;
	}

	inorder_action(tree->root, tree->action_func);
	printf("\n");
}

void levelorder(tree *tree)
{
	if (!tree) {
		return;
	}

	if (!tree->action_func) {
		printf("Invalid action function provided\n");
		return;
	}

	llist_t *queue = llist_create();
	node_t *curr = tree->root;

	while (curr) {
		tree->action_func(curr->data);

		if (curr->left) {
			llist_enqueue(queue, curr->left);
		}
		if (curr->right) {
			llist_enqueue(queue, curr->right);
		}

		curr = llist_dequeue(queue);
	}
	llist_delete(queue, NULL);
	printf("\n");
}

static void *get_max(node_t *node)
{
	while (node->right) {
		node = node->right;
	}
	return node->data;
}

void *tree_maximum(tree *tree)
{
	if (!tree->root) {
		return NULL;
	}

	return get_max(tree->root);
}

static void *get_min(node_t *node)
{
	while (node->left) {
		node = node->left;
	}
	return node->data;
}

void *tree_minimum(tree *tree)
{
	if (!tree->root) {
		return 0;
	}

	return get_min(tree->root);
}

int tree_size(tree *tree)
{
	if (!tree) {
		return 0;
	}

	return get_t_size(tree->root);
}

static void *min_node(node_t *node)
{
	while (node->left) {
		node = node->left;
	}
	return node;
}

static int delete_node(node_t **root, node_t **target, compare compare_func)
{
	node_t *node = *target;

	if (!node || !root) {
		return 0;
	}

	// Handle root node

	if (*root == *target) {
		if (!node->left && !node->right) {
			free(*root);
			*root = NULL;
			return 1;
		}

		if (!node->left || !node->right) {
			node_t *temp = node->left ? node->left : node->right;
			temp->parent = NULL;
			free(*target);
			*root = temp;
			return 1;
		}
	}

	// Handle if inner node has no right for minimum
	if (!node->right) {
		int ret = compare_func(node->parent->data, node->data);

		if (0 < ret) {
			node->parent->right = node->left;
		} else {
			node->parent->left = node->left;
		}
		if (node->left) {
			node->left->parent = node->parent;
		}
		free(node);
		return 1;
	}

	// Standard handling
	node_t *min = min_node(node->right);

	void *temp = min->data;
	min->data = node->data;
	node->data = temp;

	if (min == node->right) {
		node->right = min->right;
		if (min->right) {
			min->right->parent = node;
		}
	} else {
		min->parent->left = min->right;
		if (min->right) {
			min->right->parent = min->parent;
		}
	}

	free(min);
	return 1;
}

int tree_delete(tree **root, void *val)
{
	tree *ptr = *root;

	if (!ptr || !ptr->compare_func) {
		return 0;
	}

	node_t *search_node = tree_search(*root, val);

	if (!search_node) {
		printf("Search Value Not Found\n");
		return 0;
	}

	return delete_node(&ptr->root, &search_node, ptr->compare_func);
}

static void delete(node_t **node)
{
	if (!node || !*node) {
		return;
	}

	delete (&(*node)->left);
	delete (&(*node)->right);
	free(*node);
	*node = NULL;
}

void tree_destroy(tree **p_tree)
{
	if (!p_tree || !*p_tree) {
		return;
	}

	tree *tmp = *p_tree;

	delete (&tmp->root);
	free(*p_tree);
}

struct trunk {
	struct trunk *prev;
	const char *str;
};

static void print_trunks(struct trunk *p)
{
	if (!p) {
		return;
	}
	print_trunks(p->prev);
	printf("%s", p->str);
}

static void print_recursive(node_t *root, struct trunk *prev, int is_left)
{
	if (!root) {
		return;
	}

	struct trunk this_disp = { prev, "     " };
	const char *prev_str = this_disp.str;
	print_recursive(root->right, &this_disp, 1);

	if (!prev) {
		this_disp.str = "---";
	} else if (is_left) {
		this_disp.str = ".--";
		prev_str = "    |";
	} else {
		this_disp.str = "`--";
		prev->str = prev_str;
	}

	print_trunks(&this_disp);
	printf("%s\n", (char *)root->data); // whatever custom print you need

	if (prev) {
		prev->str = prev_str;
	}
	this_disp.str = "    |";

	print_recursive(root->left, &this_disp, 0);
	if (!prev) {
		puts("");
	}
}

void print_visual(tree *root)
{
	if (!root) {
		return;
	}
	print_recursive(root->root, NULL, 0);
}