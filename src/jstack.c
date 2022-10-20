#include <stdlib.h>
#include "jstack.h"

struct _j_stack {
    size_t length;
    size_t capacity;
    void **data;
};

JStack *j_stack_new() {
    JStack *stack = (JStack *)malloc(sizeof(JStack));

    stack->length = 0;
    stack->capacity = 1;
    stack->data = (void **)malloc(stack->capacity * sizeof(void *));

    return stack;
}

void j_stack_free(JStack *stack) {
    if(stack == NULL)
        return;

    free(stack->data);
    free(stack);
}

void j_stack_free_deep(JStack *stack, JFreeFunc func) {
    if(stack == NULL || func == NULL)
        return;

    for(size_t i = 0; i < stack->length; i++)
        func(stack->data[i]);

    free(stack->data);
    free(stack);
}

int j_stack_push(JStack *stack, void *data) {
    if(stack == NULL)
        return 0;

    // if stack buff is full, realloc the buff
    if(stack->length == stack->capacity) {
        stack->capacity *= 2;
        stack->data = (void **)realloc(stack->data, stack->capacity * sizeof(void *));
    }

    stack->data[stack->length++] = data;
    return 1;
}

void *j_stack_pop(JStack *stack) {
    if(stack == NULL || stack->length == 0)
        return NULL;

    return stack->data[--stack->length];
}

size_t j_stack_length(JStack *stack) {
    if(stack == NULL)
        return 0;

    return stack->length;
}

int j_stack_empty(JStack *stack) {
    if(stack == NULL || stack->length == 0)
        return 1;

    return 0;
}

void *j_stack_top(JStack *stack) {
    if(stack == NULL || stack->length == 0)
        return NULL;

    return stack->data[stack->length - 1];
}
