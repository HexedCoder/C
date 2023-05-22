#include <stdlib.h>
#include <pthread.h>

#include "llist.h"

typedef struct node_t {
	struct node_t *next;
	void *data;
} node_t;

struct llist_t {
	node_t *head;
	node_t *tail;
	uint64_t count;
	pthread_mutex_t mutex;
};

static node_t *create_node(void *data);

llist_t *llist_create()
{
	return calloc(1, sizeof(llist_t));
}				/* llist_create() */

static node_t *create_node(void *data)
{
	node_t *node = NULL;

	node = calloc(1, sizeof(*node));
	if (node) {
		node->data = data;
	}

	return node;
}				/* create_node() */

void llist_delete(llist_t * p_llist, void (*destroy_data)(void *))
{
	if (!p_llist) {
		return;
	}

	node_t *curr = p_llist->head;
	while (curr) {
		p_llist->head = curr->next;
		destroy_data(curr->data);
		free(curr);
		curr = p_llist->head;
	}

	free(p_llist);
}				/* llist_delete() */

int llist_push(llist_t * p_llist, void *data)
{
	int ret = 0;
	if (!p_llist || !data) {
		goto INSERT_FRONT;
	}

	node_t *node = create_node(data);

	pthread_mutex_lock(&p_llist->mutex);

	if (!node) {
		goto INSERT_FRONT;
	}

	if (!p_llist->tail) {
		p_llist->head = node;
		p_llist->tail = node;
		node->next = NULL;
	} else {
		node->next = p_llist->head;
		p_llist->head = node;
	}

	p_llist->count++;
	pthread_mutex_unlock(&p_llist->mutex);

	ret = 1;

INSERT_FRONT:
	return ret;
} /* llist_push() */

void *llist_pop(llist_t *p_llist)
{
	void *ret = NULL;

	pthread_mutex_lock(&p_llist->mutex);
	if (!p_llist->head) {
		goto EXTRACT_FRONT;
	}

	node_t *curr = p_llist->head;
	ret = curr->data;

	if (curr->next) {
		p_llist->head = curr->next;
	} else {
		p_llist->head = NULL;
		p_llist->tail = NULL;
	}
	free(curr);

	p_llist->count--;
	pthread_mutex_unlock(&p_llist->mutex);

EXTRACT_FRONT:
	return ret;
} /* llist_pop() */

int llist_enqueue(llist_t *p_llist, void *data)
{
	int ret = 0;
	if (!p_llist || !data) {
		goto INSERT_BACK;
	}

	node_t *node = create_node(data);
	pthread_mutex_lock(&p_llist->mutex);

	if (!node) {
		goto INSERT_BACK;
	}

	if (p_llist->tail) {
		p_llist->tail->next = node;
		p_llist->tail = node;
	} else {
		p_llist->head = node;
		p_llist->tail = node;
	}

	p_llist->count++;
	pthread_mutex_unlock(&p_llist->mutex);

	ret = 1;

INSERT_BACK:
	return ret;
} /* llist_enqueue() */

void *llist_dequeue(llist_t *p_llist)
{
	void *ret = llist_pop(p_llist);
	return ret;
} /* llist_dequeue() */

int llist_add_before(llist_t *p_llist, void *target, void *data)
{
	int ret = 0;
	if (!p_llist || !data || p_llist->count == 0) {
		goto INSERT_BEFORE;
	}

	node_t *node = create_node(data);

	if (!node) {
		goto INSERT_BEFORE;
	}

	node_t *curr = p_llist->head;

	if (curr->data == target) {
		node->next = curr;
		p_llist->head = node;
		ret = 1;
		goto INSERT_BEFORE;
	}

	while (curr->next && curr->next->data != target) {
		curr = curr->next;
	}
	if (curr->next) {
		if (curr->next == p_llist->tail) {
			p_llist->tail = node;
		}
		node->next = curr->next;
		curr->next = node;

		p_llist->count++;
		ret = 1;
	}

INSERT_BEFORE:
	return ret;
}				/* llist_add_before() */

int llist_add_after(llist_t * p_llist, void *target, void *data)
{
	int ret = 0;
	if (!p_llist || !data || p_llist->count == 0) {
		goto INSERT_AFTER;
	}

	node_t *node = create_node(data);

	if (!node) {
		goto INSERT_AFTER;
	}

	node_t *curr = p_llist->head;

	while (curr && curr->data != target) {
		curr = curr->next;
	}
	if (curr) {
		node->next = curr->next;
		curr->next = node;

		p_llist->count++;
		ret = 1;
	}

INSERT_AFTER:
	return ret;
}				/* llist_add_after() */

void *llist_extract_back(llist_t * p_llist)
{
	void *ret = NULL;
	if (!p_llist->tail) {
		goto EXTRACT_BACK;
	}

	node_t *curr = p_llist->head;

	if (p_llist->head == p_llist->tail) {
		ret = curr->data;

		p_llist->head = NULL;
		p_llist->tail = NULL;

		free(curr);
		p_llist->count--;
		goto EXTRACT_BACK;
	}

	while (curr->next && curr->next != p_llist->tail) {
		curr = curr->next;
	}

	if (curr->next) {
		node_t *temp = curr;

		curr = curr->next;
		temp->next = NULL;
		ret = curr->data;

		p_llist->tail = temp;
	} else {
		p_llist->head = NULL;
		p_llist->tail = NULL;
	}

	free(curr);
	p_llist->count--;

EXTRACT_BACK:
	return ret;
}				/* llist_extract_back() */

void llist_print(llist_t * p_llist, void (*print_data)(void *))
{
	if (!p_llist) {
		return;
	}

	pthread_mutex_lock(&p_llist->mutex);
	node_t *node = p_llist->head;
	while (node) {
		print_data(node->data);
		node = node->next;
	}
	pthread_mutex_unlock(&p_llist->mutex);

} /* llist_print() */

void *llist_find(llist_t *p_llist, void *target,
		 int (*compare_data)(void *initial, void *comparison))
{
	if (!p_llist || !target) {
		return NULL;
	}

	void *data = NULL;
	node_t *node = p_llist->head;
	int ret;

	while (node) {
		ret = compare_data(target, node->data);

		if (!ret) {
			data = node->data;
			break;
		}
		node = node->next;
	}
	return data;
}				/* llist_find() */

uint64_t get_size(llist_t * p_llist)
{
	return p_llist->count;
}				/* get_size() */
