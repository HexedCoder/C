#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "avl.h"
#include "llist.h"

typedef struct node_t {
	struct node_t *parent;
	struct node_t *left;
	struct node_t *right;
	int height;
	void *data;
} node_t;

struct avl_t {
	struct node_t *root;
	compare compare_func;
	action action_func;
	pthread_mutex_t mutex;
};

static node_t *create_node(void *data);

static void preorder_action(node_t * node, action action_func);

static void postorder_action(node_t * node, action action_func);

static void inorder_action(node_t * node, action action_func);

static void *get_max(node_t * node);

static void *get_min(node_t * node);

static int get_t_size(node_t * node);

avl_t *tree_create(compare compare_func, action action_func)
{
	avl_t *tree = calloc(1, sizeof(*tree));

	tree->compare_func = compare_func;
	tree->action_func = action_func;

	return tree;
}				/* tree_create() */

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
}				/* create_node() */

static int get_t_size(node_t * node)
{
	if (!node) {
		return 0;
	}

	int count = get_t_size(node->left);
	count += get_t_size(node->right);

	return 1 + count;
}

static node_t *rotate_left(avl_t ** root_tree, node_t * new_node)
{
	avl_t *ptr = *root_tree;
	node_t **root_node = &ptr->root;

	if (!new_node || new_node == *root_node ||
	    0 < ptr->compare_func(new_node->parent->data, new_node->data)) {
		return new_node;
	}

	node_t *parent = new_node->parent;
	node_t *gparent = parent->parent;
	node_t *child = new_node->left;

	new_node->parent = gparent;
	new_node->left = parent;
	parent->parent = new_node;
	parent->right = child;

	if (child) {
		child->parent = parent;
	}
	if (gparent) {
		if (ptr->compare_func(parent->data, gparent->data) < 0) {
			gparent->left = new_node;
		} else {
			gparent->right = new_node;
		}
	} else {
		// parent of rotated new_node is the root; rotated new_node
		// becomes root
		ptr->root = new_node;
	}

	return new_node;
}

static node_t *rotate_right(avl_t ** root_tree, node_t * new_node)
{
	avl_t *ptr = *root_tree;
	node_t **root_node = &ptr->root;

	if (!new_node || new_node == *root_node ||
	    ptr->compare_func(new_node->parent->data, new_node->data) < 0) {
		printf("Returned");
		return new_node;
	}
	node_t *parent = new_node->parent;
	node_t *gparent = parent->parent;
	node_t *child = new_node->right;

	new_node->parent = gparent;
	new_node->right = parent;
	parent->parent = new_node;
	parent->left = child;

	if (child) {
		child->parent = parent;
	}
	if (gparent) {
		if (ptr->compare_func(parent->data, gparent->data) < 0) {
			gparent->left = new_node;
		} else {
			gparent->right = new_node;
		}
	} else {
		// parent of rotated new_node is the root; rotated new_node
		// becomes root
		ptr->root = new_node;
	}

	return new_node;
}

static node_t *insert(node_t * tree, void *data, compare compare_func)
{
	int result = compare_func(data, tree->data);
	if (!result) {
		return NULL;
	}

	node_t *temp = NULL;

	if (result < 0) {
		if (tree->left) {
			return insert(tree->left, data, compare_func);
		} else {
			temp = create_node(data);
			if (temp) {
				temp->parent = tree;
				tree->left = temp;
			}
		}
	} else {
		if (tree->right) {
			return insert(tree->right, data, compare_func);
		} else {
			temp = create_node(data);
			if (temp) {
				temp->parent = tree;
				tree->right = temp;
			}
		}
	}

	return temp;
}				/* tree_insert() */

void *avl_insert(avl_t * tree, void *data)
{
	if (!tree || !data) {
		return NULL;
	}

	pthread_mutex_lock(&tree->mutex);
	node_t *new_node = NULL;
	if (!tree->root) {
		new_node = calloc(1, sizeof(*new_node));
		tree->root = new_node;
		if (new_node) {
			new_node->data = data;
		}
	} else {
		new_node = insert(tree->root, data, tree->compare_func);
	}
	if (!new_node) {
		return NULL;
	}
	new_node->height = 1;
	int bf = 0;

	while (new_node->parent && bf < 2) {
		new_node = new_node->parent;
		int l_bf = new_node->left ? new_node->left->height : 0;
		int r_bf = new_node->right ? new_node->right->height : 0;
		int max = (l_bf < r_bf) ? r_bf : l_bf;
		bf = abs(r_bf - l_bf);
		new_node->height = max + 1;
	}

	// no balance needed
	if (!new_node->parent && bf < 2) {
		pthread_mutex_unlock(&tree->mutex);
		return data;
	}

	if (tree->compare_func(data, new_node->data) < 0) {
		if (tree->compare_func(data, new_node->left->data) < 0) {
			new_node = rotate_right(&tree, new_node->left);
			new_node->right->height = new_node->height - 1;
		} else {
			new_node = rotate_right(&tree,
						rotate_left(&tree,
							    new_node->
							    left->right));

			if (new_node) {
				new_node->left->height = new_node->height;
				new_node->right->height = new_node->height;
				++new_node->height;
			}
		}
	} else {
		if (0 < tree->compare_func(data, new_node->right->data)) {
			new_node = rotate_left(&tree, new_node->right);
			new_node->left->height = new_node->height - 1;
		} else {
			new_node = rotate_left(&tree,
					       rotate_right(&tree,
							    new_node->
							    right->left));
			if (new_node) {
				new_node->left->height = new_node->height;
				new_node->right->height = new_node->height;
				++new_node->height;
			}
		}
	}
	pthread_mutex_unlock(&tree->mutex);

	return data;
}

static void *find(node_t * node, void *value, compare compare_func)
{
	if (!node) {
		return NULL;
	}

	int ret = compare_func(value, node->data);

	if (ret < 0) {
		return find(node->left, value, compare_func);
	} else if (ret > 0) {
		return find(node->right, value, compare_func);
	} else {
		return node;
	}
}

void *tree_search(avl_t * tree, void *value)
{
	if (!tree || !value) {
		return NULL;
	}

	return find(tree->root, value, tree->compare_func);
}

static void preorder_action(node_t * node, action action_func)
{
	if (!node) {
		return;
	}

	action_func(node->data);
	preorder_action(node->left, action_func);
	preorder_action(node->right, action_func);
}

static void postorder_action(node_t * node, action action_func)
{
	if (!node) {
		return;
	}

	postorder_action(node->left, action_func);
	postorder_action(node->right, action_func);
	action_func(node->data);
}

static void inorder_action(node_t * node, action action_func)
{
	if (!node) {
		return;
	}

	inorder_action(node->left, action_func);
	action_func(node->data);
	inorder_action(node->right, action_func);
}

void preorder(avl_t * tree)
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

void postorder(avl_t * tree)
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

void inorder(avl_t * tree)
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

void level_order(avl_t * tree)
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

static void *get_max(node_t * node)
{
	while (node->right) {
		node = node->right;
	}
	return node->data;
}

void *tree_maximum(avl_t * tree)
{
	if (!tree->root) {
		return NULL;
	}

	return get_max(tree->root);
}

static void *get_min(node_t * node)
{
	while (node->left) {
		node = node->left;
	}
	return node->data;
}

void *tree_minimum(avl_t * tree)
{
	if (!tree->root) {
		return 0;
	}

	return get_min(tree->root);
}

int tree_size(avl_t * tree)
{
	if (!tree) {
		return 0;
	}

	return get_t_size(tree->root);
}

static void *min_node(node_t * node)
{
	while (node->left) {
		node = node->left;
	}
	return node;
}

static int delete_node(node_t ** root, node_t ** target, compare compare_func)
{
	if (!target || !root) {
		return 0;
	}

	node_t *node = *target;

	//	printf("Removing %d\n", *(int *)node->data);

	// Handle root node

	if (*root == *target) {
		if (!node->left && !node->right) {
			free(node->data);
			free(*root);
			*root = NULL;
			return 1;
		}

		if (!node->left || !node->right) {
			node_t *temp = node->left ? node->left : node->right;
			//			printf("Temp Node: %s\n", (char *)temp->data);	// How to print data
			temp->parent = NULL;
			free(node->data);
			free(*target);
			*root = temp;
			return 1;
		}
	}
	// Handle if inner node has no right for minimum
	if (!node->right) {
		printf("%d Has no right\n", *(int *)node->data);
		int ret = compare_func(node->parent->data, node->data);

		if (0 < ret) {
			printf("%d is less than %d\n", *(int *)node->data,
			       *(int *)node->parent->data);
			node->parent->left = node->left;
		} else {
			node->parent->right = node->left;
		}
		if (node->left) {
			node->left->parent = node->parent;
		}
		free(node->data);
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
	free(min->data);
	free(min);
	return 1;
}

int tree_delete(avl_t ** root, void *val)
{
	avl_t *ptr = *root;

	if (!ptr || !ptr->compare_func) {
		return 0;
	}

	node_t *search_node = tree_search(*root, val);

	if (!search_node) {
		printf("Search Value Not Found\n");
		return 0;
	}

	pthread_mutex_lock(&ptr->mutex);
	int ret = delete_node(&ptr->root, &search_node, ptr->compare_func);
	pthread_mutex_unlock(&ptr->mutex);

	return ret;
}

int tree_pop(avl_t **root)
{
	avl_t *ptr = *root;

	if (!ptr) {
		return 0;
	}

	pthread_mutex_lock(&ptr->mutex);
	node_t *search_node = ptr->root;

	if (!search_node) {
		return 0;
	}

	int ret = delete_node(&ptr->root, &search_node, ptr->compare_func);
	pthread_mutex_unlock(&ptr->mutex);

	return ret;
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

void tree_destroy(avl_t ** p_tree)
{
	if (!p_tree || !*p_tree) {
		return;
	}

	avl_t *tmp = *p_tree;

	delete(&tmp->root);
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

static void print_recursive(node_t * root, struct trunk *prev, int is_left)
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
	printf("%d\n", *(int *)root->data);	// whatever custom print you need

	if (prev) {
		prev->str = prev_str;
	}
	this_disp.str = "    |";

	print_recursive(root->left, &this_disp, 0);
	if (!prev) {
		puts("");
	}
}

void print_visual(avl_t * root)
{
	if (!root) {
		return;
	}
	print_recursive(root->root, NULL, 0);
}
