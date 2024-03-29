#include "llist.h"
#include "tree.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint-gcc.h>
#include <pthread.h>

typedef struct node_t {
	struct node_t *parent;
	struct node_t *left;
	struct node_t *right;
	void *data;
} node_t;

struct tree {
	struct node_t *root;
	compare compare_func;
	action action_func;
	pthread_mutex_t mutex;
};

int num_people = 0;

static node_t *create_node(void *data);

static void preorder_action(node_t * node, action action_func);

static void postorder_action(node_t * node, action action_func);

static void inorder_action(node_t * node, action action_func);

static void *get_max(node_t * node);

static void *get_min(node_t * node);

static int get_t_size(node_t * node);

tree *tree_create(compare compare_func, action action_func)
{
	tree *tree = calloc(1, sizeof(*tree));

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
		node->data = data;
	}

	return node;
}				/* create_node() */

int tree_insert(tree * tree, void *data)
{
	int ret = 0;

	if (!tree || !data) {
		goto INSERT_EXIT;
	}

	if (!tree->compare_func) {
		printf("Null compare function given\n");
		goto INSERT_EXIT;
	}

	pthread_mutex_lock(&tree->mutex);
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
	pthread_mutex_unlock(&tree->mutex);

	ret = 1;

 INSERT_EXIT:
	return ret;
}				/* tree_insert() */

static void *find(node_t * node, void *value, compare compare_func)
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

void *tree_search(tree * tree, void *value)
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

static int get_income_action(node_t * node)
{
	if (!node) {
		return 0;
	}
	++num_people;

	person *tmp = node->data;
	int income = tmp->income;

	income += get_income_action(node->left);
	income += get_income_action(node->right);

	return income;
}

int get_income(tree * tree)
{
	if (!tree) {
		return 0;
	}

	double income = get_income_action(tree->root);

	int people = num_people;
	num_people = 0;

	if (people) {
		return income / people;
	} else {
		return 0;
	}
}

static int get_income_age_action(node_t * node, int min, int max)
{
	if (!node) {
		return 0;
	}

	int income = 0;

	income += get_income_age_action(node->left, min, max);
	income += get_income_age_action(node->right, min, max);

	person *tmp = node->data;
	if (tmp->age >= min && tmp->age <= max) {
		++num_people;
		income += tmp->income;
	} else {
		income += 0;
	}

	return income;
}

int get_age_income(tree * tree, int min, int max)
{
	if (!tree) {
		return 0;
	}

	double income = get_income_age_action(tree->root, min, max);
	int people = num_people;
	num_people = 0;

	if (people) {
		return income / people;
	} else {
		return 0;
	}
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

void preorder(tree * tree)
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

void postorder(tree * tree)
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

void inorder(tree * tree)
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

void levelorder(tree * tree)
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

void *tree_maximum(tree * tree)
{
	if (!tree->root) {
		return NULL;
	}

	person *tmp = get_max(tree->root);

	return tmp->ssn;
}

static void *get_min(node_t * node)
{
	while (node->left) {
		node = node->left;
	}
	return node->data;
}

void *tree_minimum(tree * tree)
{
	if (!tree->root) {
		return 0;
	}

	person *tmp = get_min(tree->root);

	return tmp->ssn;
}

static int get_t_size(node_t * node)
{
	if (!node) {
		return 0;
	}

	int count = get_t_size(node->left);
	count += get_t_size(node->right);

	return 1 + count;
}

int tree_size(tree * tree)
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
			printf("Temp Node: %s\n", (char *)temp->data);	// How to print data
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

int tree_delete(tree ** root, void *val)
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

static void delete(node_t ** node)
{
	if (!node || !*node) {
		return;
	}

	node_t *tmp = *node;
	person *data = tmp->data;
	delete(&(*node)->left);
	delete(&(*node)->right);

	free(data->ssn);
	free(data->fname);
	free(data->lname);
	free(data);
	free(*node);
	*node = NULL;
}

void tree_destroy(tree ** p_tree)
{
	if (!p_tree || !*p_tree) {
		return;
	}

	tree *tmp = *p_tree;

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
	person *tmp = root->data;
	printf("%d\n", tmp->age);	// whatever custom print you need

	if (prev) {
		prev->str = prev_str;
	}
	this_disp.str = "    |";

	print_recursive(root->left, &this_disp, 0);
	if (!prev) {
		puts("");
	}
}

void print_visual(tree * root)
{
	if (!root) {
		return;
	}
	print_recursive(root->root, NULL, 0);
}
