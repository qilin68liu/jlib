#include <stdlib.h>
#include "jprequeue.h"

#define PA(i) (((i) - 1) / 2) // parent
#define LE(i) ((i) * 2 + 1)   // left
#define RI(i) ((i) * 2 + 2)   // right

struct _j_prequeue {
    size_t length;
    size_t capacity;

    void **data;
    JCompareFunc comp_func;
};

static void heapify(JPreQueue *prequeue, size_t i);

JPreQueue *j_prequeue_new(JCompareFunc func) {
    JPreQueue *prequeue = (JPreQueue *)malloc(sizeof(JPreQueue));

    prequeue->length = 0;
    prequeue->capacity = 1;
    prequeue->comp_func = func;
    prequeue->data = (void **)malloc(prequeue->capacity * sizeof(void *));

    return prequeue;
}

void j_prequeue_free(JPreQueue *prequeue) {
    if (prequeue != NULL) {
        free(prequeue->data);
        free(prequeue);
    }
}

void j_prequeue_free_deep(JPreQueue *prequeue, JFreeFunc func) {
    if (prequeue == NULL || func == NULL) {
        return;
    }

    for (size_t i = 0; i < prequeue->length; ++i) {
        func(prequeue->data[i]);
    }

    free(prequeue->data);
    free(prequeue);
}

void *j_prequeue_pop(JPreQueue *prequeue) {
    if (prequeue == NULL || prequeue->length == 0) {
        return NULL;
    }

    void *result = prequeue->data[0];
    prequeue->data[0] = prequeue->data[prequeue->length - 1];
    --prequeue->length;
    heapify(prequeue, 0);

    return result;
}

int j_prequeue_push(JPreQueue *prequeue, void *data) {
    if (prequeue == NULL) {
        return 0;
    }

    if (prequeue->length == prequeue->capacity) {
        prequeue->capacity *= 2;
        prequeue->data = (void **)realloc(prequeue->data, prequeue->capacity * sizeof(void *));
    }

    size_t new_index = prequeue->length++;
    prequeue->data[new_index] = data;

    while (new_index > 0) {
        size_t parent = PA(new_index);
        if (!prequeue->comp_func(prequeue->data[new_index], prequeue->data[parent])) {
            break;
        }
        void *tmp = prequeue->data[parent];
        prequeue->data[parent] = prequeue->data[new_index];
        prequeue->data[new_index] = tmp;
        new_index = parent;
    }

    return 1;
}

size_t j_prequeue_length(JPreQueue *prequeue) {
    return (prequeue == NULL ? 0 : prequeue->length);
}

int j_prequeue_empty(JPreQueue *prequeue) {
    if (prequeue == NULL || prequeue->length == 0) {
        return 1;
    }

    return 0;
}

void *j_prequeue_top(JPreQueue *prequeue) {
    if (prequeue == NULL || prequeue->length == 0) {
        return NULL;
    }

    return prequeue->data[0];
}

static void heapify(JPreQueue *prequeue, size_t i) {
    if (prequeue == NULL || prequeue->length <= 1 || i > prequeue->length - 1) {
        return;
    }

    size_t cand = i; // candidate

    size_t left = LE(i);
    if (left < prequeue->length && prequeue->comp_func(prequeue->data[left], prequeue->data[cand])) {
        cand = left;
    }

    size_t right = RI(i);
    if (right < prequeue->length && prequeue->comp_func(prequeue->data[right], prequeue->data[cand])) {
        cand = right;
    }

    if (cand != i) {
        void *tmp = prequeue->data[i];
        prequeue->data[i] = prequeue->data[cand];
        prequeue->data[cand] = tmp;
        heapify(prequeue, cand);
    }
}
