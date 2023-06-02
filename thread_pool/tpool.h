/** @file tpool.h
*
* @brief This module serves as the template for functions in tpool.c
*
* @par
* COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
* inspired by @cite https://youtu.be/_n2hE2gyPxU
*/
#ifndef TPOOL_H
#define TPOOL_H

typedef struct tpool tpool_t;
typedef void *(*task_func_t)(void *);

/**
* @brief Allocate and initialize a thread pool with the specified number of worker threads.
*
* @param num_workers: The number of worker threads to create.
*
* @return Pointer to the newly created thread pool, or NULL on error.
*/
tpool_t *tpool_create(int num_workers);

/**
* @brief Add a new task to the thread pool's task queue.
*
* @param tpool: Pointer to the thread pool to which the task should be added.
* @param task: Function pointer to the task to be added.
* @param arg: An argument to be passed to the task function.
*/
void tpool_enqueue(tpool_t * tpool, task_func_t task, void *arg);

/**
* @brief Shut down the thread pool and clean up all resources.
*
* @param tpool: A pointer to the thread pool to shut down.
*/
void tpool_shutdown(tpool_t ** tpool);

#endif				/* TPOOL_H */

/*** end of file ***/
