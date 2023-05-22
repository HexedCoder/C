#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "pqueue.h"

#define NODES 10000
#define NUM_THREADS 10
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int nodes_left = 0;

void *pop_root(void *arg)
{
	while (!pqueue_is_empty(arg)) {
		pthread_mutex_lock(&mutex);
		int *key = pqueue_extract(arg);
		if (key) {
			free(key);
			printf("Nodes_left: %d\n", --nodes_left);
		}
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

int main()
{
	pqueue_t *queue = pqueue_create(NODES, free);

	if (!queue) {
		fprintf(stderr, "Could not create min-queue\n");
		return 1;
	}

	for (int i = 0; i < NODES; ++i) {
		char *key = malloc(sizeof(char));
		int priority = rand() % 128;
		*key = rand() % 26 + 64;
		pqueue_insert(queue, key, priority);
		++nodes_left;
		printf("Nodes: %d\n", nodes_left);
	}

	pthread_t threads[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_create(&threads[i], NULL, pop_root, queue);
	}

	// wait for threads to finish
	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_join(threads[i], NULL);
	}

	fprintf(stderr, "\n");

	pqueue_delete(queue);
}
