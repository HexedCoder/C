#include "tpool.h"

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct task {
	task_func_t function;
	void *argument;
	struct task *next;
} task_t;

struct tpool {
	int num_workers;
	pthread_t *workers;
	task_t *task_queue;
	pthread_mutex_t queue_mutex;
	pthread_cond_t queue_condition;
	bool shutdown;
};

static void *worker_thread(void *arg)
{
	tpool_t *tpool = arg;
	while (true) {
		// wait for task enqueue or shutdown
		pthread_mutex_lock(&tpool->queue_mutex);

		while (!tpool->task_queue && !tpool->shutdown) {
			pthread_cond_wait(&tpool->queue_condition,
					  &tpool->queue_mutex);
		}

		// if shutdown, exit the thread
		if (tpool->shutdown) {
			pthread_mutex_unlock(&tpool->queue_mutex);
			pthread_exit(NULL);
		}
		// get next task from queue
		task_t *task = tpool->task_queue;

		if (!task) {
			// release mutex
			pthread_mutex_unlock(&tpool->queue_mutex);
			continue;
		}
		tpool->task_queue = task->next;

		// unlock mutex
		pthread_mutex_unlock(&tpool->queue_mutex);

		// execute the function
		task->function(task->argument);

		// free task
		free(task);
	}
}				/* worker_thread() */

tpool_t *tpool_create(int num_workers)
{
	if (num_workers < 1) {
		return NULL;
	}
	// allocate struct
	tpool_t *tpool = malloc(sizeof(tpool_t));
	if (!tpool) {
		return NULL;
	}

	tpool->num_workers = num_workers;
	tpool->workers = malloc(sizeof(pthread_t) * num_workers);
	if (!tpool->workers) {
		free(tpool);
		return NULL;
	}

	tpool->task_queue = NULL;
	pthread_mutex_init(&tpool->queue_mutex, NULL);
	pthread_cond_init(&tpool->queue_condition, NULL);
	tpool->shutdown = false;

	// create threads
	for (int i = 0; i < num_workers; i++) {
		pthread_create(&tpool->workers[i], NULL, worker_thread, tpool);
	}

	return tpool;
}				/* tpool_create() */

void tpool_enqueue(tpool_t * tpool, task_func_t task, void *arg)
{
	if (!tpool || !task || tpool->shutdown) {
		return;
	}
	// allocate memory for new task
	task_t *new_task = malloc(sizeof(task_t));
	if (!new_task) {
		return;
	}
	// initialize variables
	new_task->function = task;
	new_task->argument = arg;
	new_task->next = NULL;

	// lock mutex
	pthread_mutex_lock(&tpool->queue_mutex);

	// add new task
	if (!tpool->task_queue) {
		tpool->task_queue = new_task;
	} else {

		task_t *current_task = tpool->task_queue;

		while (current_task->next != NULL) {
			current_task = current_task->next;
		}
		current_task->next = new_task;
	}

	// signal thread to process new task
	pthread_cond_signal(&tpool->queue_condition);

	// unlock mutex
	pthread_mutex_unlock(&tpool->queue_mutex);
}				/* tpool_enqueue() */

void tpool_shutdown(tpool_t ** tpool_ptr)
{
	if (!tpool_ptr || !*tpool_ptr) {
		return;
	}

	tpool_t *tpool = *tpool_ptr;
	// lock mutex
	pthread_mutex_lock(&tpool->queue_mutex);

	// signal threads to shut down
	tpool->shutdown = true;
	pthread_cond_broadcast(&tpool->queue_condition);

	// unlock mutex
	pthread_mutex_unlock(&tpool->queue_mutex);
	pthread_t *ptr = &tpool->workers[0];

	// wait on all threads before exit
	for (int i = 0; i < tpool->num_workers; i++) {
		pthread_join(*ptr++, NULL);
	}

	// free thread array
	free(tpool->workers);
	tpool->workers = NULL;
	tpool->num_workers = 0;

	// free all tasks in queue
	task_t *current_task = tpool->task_queue;
	while (current_task != NULL) {
		task_t *next_task = current_task->next;
		free(current_task);
		current_task = next_task;
	}

	tpool->task_queue = NULL;

	// Destroy the mutex and condition variable
	pthread_mutex_destroy(&tpool->queue_mutex);
	pthread_cond_destroy(&tpool->queue_condition);

	// free struct
	free(tpool);
	*tpool_ptr = NULL;
}				/* tpool_shutdown() */
