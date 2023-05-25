/**
* @file llist_driver.c
* @author Jacob Hitchcox
* @brief
*
* COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
*
*/

#include "llist.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct node_t node_t;

struct node_t {
	char value;
};

#define MAX_NODES 10000
#define MAX_THREADS 10

struct node_t *node_create(char value)
{
	node_t *node = calloc(1, sizeof(*node));

	if (!value) {
		goto PERSON_CREATE_EXIT;
	}

	if (node) {
		node->value = value;
	}
PERSON_CREATE_EXIT:
	return node;
}

void *pop_nodes(void *arg)
{
	llist_t *list = (llist_t *)arg;
	for (int i = 0; i < 1000; ++i) {
		void *node = llist_dequeue(list);
		if (node) {
			free(node);
			node = NULL;
		} else {
			break;
		}
	}
	return NULL;
}

int main(void)
{
	llist_t *list = llist_create();
	if (!list) {
		perror("queue creation");
		return -1;
	}

	char element = 'A'; // random number for each node

	// populate linked list with max nodes
	for (int i = 0; i < MAX_NODES; ++i) {
		node_t *new_node = node_create(((element + i) % 26) + i);
		// TODO: ABC's for new node

		llist_enqueue(list, new_node);
	}
	// get size of linked list
	printf("initial llist size: %ld\n", get_size(list));

	// create array of 10 thread pointers
	pthread_t threads[MAX_THREADS];
	for (int i = 0; i < MAX_THREADS; ++i) {
		pthread_create(&threads[i], NULL, pop_nodes, list);
	}

	// wait for threads to finish
	for (int i = 0; i < MAX_THREADS; ++i) {
		pthread_join(threads[i], NULL);
	}

	printf("(after threads) llist size: %ld\n", get_size(list));

	// free memory
	llist_delete(list, NULL);

	return 0;
}

// notes:
// create 10,000 ll nodes
// have 'thead' function to pop off 1000 nodes
// for each ten, create thread, with thread_pop function
// for each ten join thread
// display size of linked list

/*** end of file ***/