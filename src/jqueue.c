#include <stdlib.h>
#include "jqueue.h"

struct _j_queue {
    size_t length;
    size_t capacity;
    size_t head;
    size_t tail;
    void **data;
};

JQueue *j_queue_new() {
    JQueue *queue = (JQueue *)malloc(sizeof(JQueue));

    queue->length = 0;
    queue->capacity = 1;
    queue->head = 0;
    queue->tail = 0;
    queue->data = (void **)malloc(queue->capacity * sizeof(void *));

    return queue;
}

void j_queue_free(JQueue *queue) {
    if(queue == NULL)
        return;

    free(queue->data);
    free(queue);
}

void j_queue_free_deep(JQueue *queue, JFreeFunc func) {
    if(queue == NULL || func == NULL)
        return;

    while(queue->length != 0)
        func(j_queue_dequeue(queue));

    free(queue->data);
    free(queue);
}

int j_queue_enqueue(JQueue *queue, void *data) {
    if(queue == NULL)
        return 0;

    // if queue buff is full, realloc the buff
    if(queue->length == queue->capacity) {
        size_t to_add = queue->capacity;
        queue->capacity += to_add;
        queue->data = (void **)realloc(queue->data, queue->capacity * sizeof(void *));

        // if tail < head, which means we need to expand buff between tail and head
        // by moving items start from head to the end of buff
        if(queue->tail < queue->head) {
            queue->head += to_add;
            for(size_t i = queue->head, j = queue->head - to_add; i < queue->capacity; i++, j++)
                queue->data[i] = queue->data[j];
        }
    }

    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->data[queue->tail] = data;

    ++queue->length;

    return 1;
}

void *j_queue_dequeue(JQueue *queue) {
    if(queue == NULL || queue->length == 0)
        return NULL;

    void *data = queue->data[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;

    --queue->length;

    return data;
}

size_t j_queue_length(JQueue *queue) {
    if(queue == NULL)
        return 0;

    return queue->length;
}

int j_queue_empty(JQueue *queue) {
    if(queue == NULL || queue->length == 0)
        return 1;

    return 0;
}

void *j_queue_head(JQueue *queue) {
    if(queue == NULL || queue->length == 0)
        return NULL;

    return queue->data[queue->head];
}

void *j_queue_tail(JQueue *queue) {
    if(queue == NULL || queue->length == 0)
        return NULL;

    return queue->data[queue->tail];
}
