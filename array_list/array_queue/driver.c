#include <stdio.h>

#include "queue.h"

#define QUEUE_MAX 10

int main()
{
	queue_t *queue = queue_create(QUEUE_MAX);

	for (int i = -1; i < QUEUE_MAX + 1; ++i) {
		if (!queue_enqueue(queue, i)) {
			fprintf(stderr, "Could not add %d to queue\n", i);
		}
	}

	while (!queue_is_empty(queue)) {
		printf("Extracted %d\n", queue_dequeue(queue));
	}

	queue_destroy(&queue);
}
