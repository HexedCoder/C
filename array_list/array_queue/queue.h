/** @file queue.h
 *
 * @brief Queue implemented as an array to store values greater than 0
 */
#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue_t queue_t;

/*
 * @brief Creates a queue of size 'size'
 *
 * @param int Maximum capacity of queue
 * @return Memory address of heap allocated queue or NULL
 */
queue_t *queue_create(int size);

/*
 * @brief Properly disposes of queue_t
 *
 * @param **queue_t Reference to location queue * is stored
 */
void queue_destroy(queue_t ** queue);

/*
 * @brief Resets queue to empty
 *
 * @param *queue_t Address of queue to reset
 * @return None
 */
void queue_reset(queue_t * queue);

/*
 * @brief Enqueues an 'int' to the queue
 *
 * @param *queue_t Address of queue to load value into
 * @param int The value desired to be put in queue
 *
 * @return *queue_t Address of queue on success, NULL on failure
 */
queue_t *queue_enqueue(queue_t * queue, int value);

/*
 * @brief Recovers next item in queue
 *
 * @param *queue_t Address of queue to extract value from
 * @return int Value stored in queue, -1 on failure
 */
int queue_dequeue(queue_t * queue);

/*
 * @brief Recovers the head value of the queue
 *
 * @param *queue_t Address of queue to peek at value of
 * @return int Value stored at head of queue
 */
int queue_peek(queue_t * queue);

/*
 * @brief Modifies the head value of the queue
 *
 * @param *queue_t Address of queue to modify head value of
 * @param int Value to change the head to
 *
 * @return int Previous value stored in queue, else -1 on failure
 */
int queue_change_head(queue_t * queue, int value);

/*
 * @brief Evaluates if queue is empty
 *
 * @param *queue_t Address of queue to evaluate
 * 
 * @return *queue_t Address of queue on success, NULL on failure
 */
queue_t *queue_is_empty(queue_t * queue);

#endif
