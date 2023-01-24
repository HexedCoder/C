/** @file pqueue.c
*
* @brief This module implements the functions in pqueue.h
* @par
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*/

#include "pqueue.h"
#include "kdtree_funcs.h"

#include <stdio.h>
#include <stdlib.h>

struct pqueue_t {
	delete_func delete;
	int count;
	int capacity;
	node_t *back;
	node_t *heap;
};

pqueue_t *pqueue_create(int capacity, delete_func delete)
{
	pqueue_t *pqueue = malloc(sizeof(*pqueue));
	if (!pqueue) {
		goto PQUEUE_CREATE_EXIT;
	}

	pqueue->heap = calloc(capacity, sizeof(node_t));
	if (!pqueue->heap) {
		printf("Unable to allocate memory\n");
		free(pqueue);
		goto PQUEUE_CREATE_EXIT;
	}

	pqueue->count = 0;
	pqueue->delete = delete;
	pqueue->back = pqueue->heap;
	pqueue->capacity = capacity;

 PQUEUE_CREATE_EXIT:
	return pqueue;
}				/* pqueue_create() */

void pqueue_destroy(pqueue_t * pqueue)
{
	if (!pqueue) {
		printf("Empty pqueue provided\n");
		return;
	}

	if (!pqueue_is_empty(pqueue)) {
		node_t *temp = pqueue->heap;

		while (temp != pqueue->back) {
			temp->node_data = NULL;
			++temp;
		}
	}

	free(pqueue->heap);
	pqueue->back = NULL;
	pqueue->heap = NULL;
	pqueue->count = 0;
	pqueue->capacity = 0;

	free(pqueue);
}				/* pqueue_destroy() */

static void heapify_up(int position, node_t * heap)
{
	int32_t parent = (position - 1) / 2;

	if (parent < 0) {
		return;
	}

	if (heap[parent].priority > heap[position].priority) {
		node_t temp = heap[parent];
		heap[parent] = heap[position];
		heap[position] = temp;
		heapify_up(parent, heap);
	}
}				/* heapify_up() */

int pqueue_insert(pqueue_t * pqueue, void *item, double priority)
{
	if (!pqueue || !pqueue->heap) {
		return 0;
	}

	if (pqueue_is_full(pqueue)) {
		printf("Queue is full, unable to add item\n");
	} else {
		pqueue->heap[pqueue->count].node_data = item;
		pqueue->heap[pqueue->count].priority = priority;

		heapify_up(pqueue->count++, pqueue->heap);

		++pqueue->back;
	}

	return 1;

}				/* pqueue_insert() */

static void heapify_down(int position, int size, node_t * heap)
{
	int left_child = 2 * position + 1;
	int right_child = 2 * position + 2;

	if (left_child >= size) {
		return;
	}

	int swap_idx = 0;

	if (heap[left_child].priority < heap[right_child].priority) {
		swap_idx = left_child;
	} else {
		swap_idx = right_child;
	}

	if (heap[position].priority > heap[swap_idx].priority) {
		node_t tmp = heap[swap_idx];
		heap[swap_idx] = heap[position];
		heap[position] = tmp;

		heapify_down(swap_idx, size, heap);
	}
}				/* heapify_down() */

void *pqueue_extract(pqueue_t * pqueue)
{
	if (!pqueue | !pqueue->heap) {
		return NULL;
	}

	void *temp = NULL;
	if (pqueue_is_empty(pqueue)) {
		fprintf(stderr, "Queue is empty\n");
	} else {
		temp = pqueue->heap[0].node_data;
		pqueue->count--;
		pqueue->back--;
		pqueue->heap[0] = pqueue->heap[pqueue->count];
		heapify_down(0, pqueue->count, pqueue->heap);
	}

	return temp;
}				/* pqueue_extract() */

int pqueue_is_empty(pqueue_t * pqueue)
{
	if (!pqueue) {
		return 1;
	} else {
		return 0 == pqueue->count;
	}
}				/* pqueue_is_empty() */

bool pqueue_is_full(pqueue_t * pqueue)
{
	if (pqueue->capacity == pqueue->count) {
		return true;
	}

	return false;

}				/* pqueue_is_full() */

void pqueue_print(pqueue_t * pqueue)
{
	node_t node = pqueue->heap[0];

	tree *data = node.node_data;
	printf("Distance: %lf from (%lf, %lf)\n", node.priority, data->x_coord,
	       data->y_coord);
}

/*** END OF FILE ***/
