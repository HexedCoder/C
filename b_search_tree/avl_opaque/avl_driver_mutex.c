#include "avl.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 5

typedef struct
{
    avl_t *tree;
    int    start;
    int    end;
} thread_arg_t;

int
int_cmp(const void *input_1, const void *input_2)
{
    return *(int *)input_1 - *(int *)input_2;
}

void *
pop_root(void *arg)
{
    thread_arg_t *thread_arg = (thread_arg_t *)arg;
    for (int i = thread_arg->start; i < thread_arg->end; ++i)
    {
        int val  = i + 1;
        int node = avl_node_delete(&thread_arg->tree, &val, free);
        if (!node)
        {
            break;
        }
    }
    return NULL;
}

int
main(void)
{
    int    arr_size = 10000;
    avl_t *tree     = avl_tree_create((int (*)(void *, void *))int_cmp, NULL);
    int    ret      = 0;
    for (int i = 0; i < arr_size; ++i)
    {
        int *num = calloc(1, sizeof(int));
        *num     = i + 1;
        ret      = *(int *)avl_insert(tree, num);
        if (!ret)
        {
            return 1;
        }
    }

    printf("Size: %d\n", avl_tree_size(tree));

    pthread_t    threads[NUM_THREADS];
    thread_arg_t args[NUM_THREADS];
    int          range = arr_size / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        args[i].tree  = tree;
        args[i].start = i * range;
        args[i].end   = (i + 1) * range;
        pthread_create(&threads[i], NULL, pop_root, &args[i]);
    }

    // wait for threads to finish
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Size: %d\n", avl_tree_size(tree));

    avl_tree_destroy(&tree, free);
}
