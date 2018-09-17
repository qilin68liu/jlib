#include <stdlib.h>
#include "jstack.h"

typedef struct _j_snode JSNode;

struct _j_snode {
    void *data;
    JSNode *next;
};

struct _j_stack {
    size_t length;
    JSNode *head;
};

static JSNode *new_node(void *data);
static void free_node(JSNode **node);

JStack *j_stack_new()
{
    JStack *stack = (JStack *)malloc(sizeof(JStack));

    stack->length = 0;
    stack->head = NULL;

    return stack;
}

void j_stack_free(JStack **stack)
{
    if(stack == NULL)
        return;

    JSNode *cur = (*stack)->head;
    JSNode *del = NULL;
    while(cur != NULL)
    {
        del = cur;
        cur = cur->next;
        free_node(&del);
    }

    free(*stack);
    *stack = NULL;
}

void j_stack_free_deep(JStack **stack, JFreeFunc func)
{
    if(stack == NULL || func == NULL)
        return;

    JSNode *cur = (*stack)->head;
    JSNode *del = NULL;
    while(cur != NULL)
    {
        del = cur;
        cur = cur->next;
        func(&(del->data));
        free_node(&del);
    }

    free(*stack);
    *stack = NULL;
}

int j_stack_push(JStack *stack, void *data)
{
    if(stack == NULL)
        return 0;

    JSNode *node = new_node(data);
    if(stack->length == 0)
    {
        stack->head = node;
    }
    else
    {
        node->next = stack->head;
        stack->head = node;
    }
    stack->length++;
    return 1;
}

void *j_stack_pop(JStack *stack)
{
    if(stack == NULL || stack->length == 0)
        return NULL;

    JSNode *del = stack->head;
    void *data = del->data;
    stack->head = del->next;
    free_node(&del);
    stack->length--;

    return data;
}

size_t j_stack_length(const JStack *stack)
{
    if(stack == NULL)
        return 0;

    return stack->length;
}

int j_stack_empty(const JStack *stack)
{
    if(stack == NULL || stack->length == 0)
        return 1;

    return 0;
}

void *j_stack_top(const JStack *stack)
{
    if(stack == NULL || stack->length == 0)
        return NULL;

    return stack->head->data;
}

static JSNode *new_node(void *data)
{
    JSNode *node = (JSNode *)malloc(sizeof(JSNode));

    node->data = data;
    node->next = NULL;

    return node;
}

static void free_node(JSNode **node)
{
    free(*node);
    *node = NULL;
}
