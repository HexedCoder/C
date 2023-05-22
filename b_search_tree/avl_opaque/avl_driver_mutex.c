#include "avl.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

int int_cmp(const void *input_1, const void *input_2)
{
	return *(int *)input_1 - *(int *)input_2;
}

void *pop_root(void *arg)
{
	avl_t *tree = (avl_t *)arg;
	for (int i = 0; i < (10000 / NUM_THREADS); ++i) {
		int node = tree_pop(&tree);
		if (!node) {
			break;
		}
	}
	return NULL;
}

int main(void)
{
	int arr_size = 10000;

	avl_t *tree = tree_create((int (*)(void *, void *))int_cmp,
				  (void (*)(void *))NULL);
	int ret = 0;
	for (int i = 0; i < arr_size; ++i) {
		int *num = calloc(1, sizeof(int *));
		*num = i + 1;
		ret = *(int *)avl_insert(tree, num);
		if (!ret) {
			return 1;
		}
	}

	printf("Size: %d\n", tree_size(tree));

	pthread_t threads[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_create(&threads[i], NULL, pop_root, tree);
	}

	// wait for threads to finish
	for (int i = 0; i < NUM_THREADS; ++i) {
		pthread_join(threads[i], NULL);
	}

	printf("Size: %d\n", tree_size(tree));

	tree_destroy(&tree);
}
