#include <stdlib.h>

#include "queue.h"

struct queue_t {
    int head;
    int tail;
    int size;
    int *data;
};

queue_t *queue_create(int size) {
    if (size < 1) {
        return NULL;
    }

    queue_t *queue = calloc(1, sizeof(*queue));
    if (queue) {
        queue->size = size;
        queue->data = calloc(size, sizeof(int));
        if (!queue->data) {
            free(queue);
            queue = NULL;
        }
    }

    return queue;
}

void queue_destroy(queue_t **queue) {
    if (!queue || !*queue) {
        return;
    }

    free((*queue)->data);
    free(*queue);
    *queue = NULL;
}

void queue_reset(queue_t *queue) {
    if (!queue) {
        return;
    }
    queue->head = 0;
    queue->tail = 0;
}

queue_t *queue_enqueue(queue_t *queue, int value) {
    // avoids negative numbers, or max capacity reached
    if (!queue || value < 0 || queue->tail == queue->size) {
        return NULL;
    }

    queue->data[queue->tail++] = value;

    return queue;
}

int queue_dequeue(queue_t *queue) {
    // handles NULL queue and empty queue
    if (!queue || queue->head == queue->tail) {
        return -1;
    }

    return queue->data[queue->head++];
}

int queue_peek(queue_t *queue) {
    // handles NULL queue and empty queue
    if (!queue || queue->head == queue->tail) {
        return -1;
    }

    return queue->data[queue->head];
}

int queue_change_head(queue_t *queue, int value) {
    // handles NULL queue and empty queue and invalid value
    if (!queue || queue->head == queue->tail || value < 0) {
        return -1;
    }

    int old_value = queue->data[queue->head];
    queue->data[queue->head] = value;
    return old_value;
}

queue_t *queue_is_empty(queue_t *queue) {
    // handles NULL queue and if head != tail, then items still in queue
    if (!queue || queue->head != queue->tail) {
        return NULL;
    }

    return queue;
}