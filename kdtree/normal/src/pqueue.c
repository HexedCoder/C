#include "pqueue.h"
#include "kdtree.h"

#include <stdio.h>
#include <stdlib.h>

// Used after insertion to organize queue
//
static void heapify_up(uint16_t position, node_t * heap);

// Used after extraction to reorganize queue
//
static void heapify_down(uint16_t position, uint16_t size, node_t * heap);

pqueue_t *pqueue_create(uint16_t capacity, del_f delete)
{
	pqueue_t *pqueue = malloc(sizeof(pqueue_t));
	if (!pqueue) {
		return NULL;
	}

	pqueue->heap = calloc(capacity, sizeof(node_t));
	if (!pqueue->heap) {
		fprintf(stderr,
			"[!]ERR: Could not create heap. Try smaller capacity");
		free(pqueue);
		return NULL;
	}

	pqueue->count = 0;
	pqueue->delete = delete;
	pqueue->back = pqueue->heap;
	pqueue->capacity = capacity;

	return pqueue;
}				/* pqueue_create() */

void pqueue_delete(pqueue_t * pqueue)
{
	if (!pqueue) {
		fprintf(stderr, "Could not destroy queue");
		return;
	}

	{
		if (!pqueue_is_empty(pqueue)) {
			node_t *temp = pqueue->heap;
			do {
				temp->node_data = NULL;
				++temp;
			} while (temp != pqueue->back);
		}
		free(pqueue->heap);
		pqueue->back = NULL;
		pqueue->heap = NULL;
		pqueue->count = 0;
		pqueue->capacity = 0;
	}
	free(pqueue);
}				/* pqueue_delete() */

static void heapify_up(uint16_t position, node_t * heap)
{
	int32_t parent = (position - 1) / 2;
	if (parent < 0) {
		return;
	}

	node_t temp;
	if (heap[parent].priority > heap[position].priority) {
		temp = heap[parent];
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

	{
		if (pqueue_is_full(pqueue)) {
			fprintf(stderr, "Queue is full\n");
		} else {
			pqueue->heap[pqueue->count].node_data = item;
			pqueue->heap[pqueue->count].priority = priority;
			heapify_up(pqueue->count, pqueue->heap);
			pqueue->back++;
			pqueue->count++;
		}
	}
	return 1;

}				/* pqueue_insert() */

// Used after extraction to reorganize queue
//
static void heapify_down(uint16_t position, uint16_t size, node_t * heap)
{
	uint16_t left_child = 2 * position + 1;
	uint16_t right_child = 2 * position + 2;

	if (left_child >= size) {
		return;
	}

	uint16_t swap_idx = heap[left_child].priority <
	    heap[right_child].priority ? left_child : right_child;

	if (heap[position].priority > heap[swap_idx].priority) {
		node_t temp = heap[swap_idx];
		heap[swap_idx] = heap[position];
		heap[position] = temp;

		heapify_down(swap_idx, size, heap);
	}
}				/* heapify_down() */

void *pqueue_extract(pqueue_t * pqueue)
{
	if (!pqueue | !pqueue->heap) {
		return NULL;
	}

	void *temp;
	{
		if (pqueue_is_empty(pqueue)) {
			fprintf(stderr, "Extraction failed, queue is empty");
		}

		temp = pqueue->heap[0].node_data;
		pqueue->count--;
		pqueue->back--;
		pqueue->heap[0] = pqueue->heap[pqueue->count];
		heapify_down(0, pqueue->count, pqueue->heap);
	}
	return temp;
}				/* pqueue_extract() */

bool pqueue_is_empty(pqueue_t * pqueue)
{
	return !pqueue || pqueue->count == 0;
}				/* pqueue_is_empty() */

bool pqueue_is_full(pqueue_t * pqueue)
{
	return pqueue->capacity == pqueue->count;
}				/* pqueue_is_full() */

void pqueue_print(pqueue_t * pqueue)
{
	node_t node = pqueue->heap[0];

	tree *data = node.node_data;
	printf("Distance: %lf from (%lf, %lf)\n", node.priority, data->x_coord,
	       data->y_coord);
}

/*** END OF FILE ***/
