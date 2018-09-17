#include <stdlib.h>
#include "jqueue.h"

typedef struct _j_qnode JQNode;

struct _j_qnode {
    void *data;
    JQNode *next;
};

struct _j_queue {
    size_t length;
    JQNode *head;
    JQNode *tail;
};

static JQNode *new_node(void *data);
static void free_node(JQNode **node);

JQueue *j_queue_new()
{
    JQueue *queue = (JQueue *)malloc(sizeof(JQueue));

    queue->length = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void j_queue_free(JQueue **queue)
{
    if(*queue == NULL)
        return;

    JQNode *cur = (*queue)->head;
    JQNode *del = NULL;
    while(cur != NULL)
    {
        del = cur;
        cur = cur->next;
        free_node(&del);
    }

    free(*queue);
    *queue = NULL;
}

void j_queue_free_deep(JQueue **queue, JFreeFunc func)
{
    if(*queue == NULL || func == NULL)
        return;

    JQNode *cur = (*queue)->head;
    JQNode *del = NULL;
    while(cur != NULL)
    {
        del = cur;
        cur = cur->next;
        func(&(del->data));
        free_node(&del);
    }

    free(*queue);
    *queue = NULL;
}

int j_queue_enqueue(JQueue *queue, void *data)
{
    if(queue == NULL)
        return 0;

    JQNode *node = new_node(data);
    
    if(queue->length == 0)
    {
        queue->head = node;
        queue->tail = node;
    }
    else
    {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->length++;
    return 1;
}

void *j_queue_dequeue(JQueue *queue)
{
    if(queue == NULL || queue->length == 0)
        return NULL;

    JQNode *del = queue->head;
    void *data = del->data;
    queue->head = del->next;
    free_node(&del);
    queue->length--;

    return data;
}

size_t j_queue_length(const JQueue *queue)
{
    if(queue == NULL)
        return 0;

    return queue->length;
}

int j_queue_empty(const JQueue *queue)
{
    if(queue == NULL || queue->length == 0)
        return 1;

    return 0;
}

void *j_queue_head(const JQueue *queue)
{
    if(queue == NULL || queue->length == 0)
        return NULL;

    return queue->head->data;
}

void *j_queue_tail(const JQueue *queue)
{
    if(queue == NULL || queue->length == 0)
        return NULL;

    return queue->tail->data;
}

static JQNode *new_node(void *data)
{
    JQNode *node = (JQNode *)malloc(sizeof(JQNode));

    node->data = data;
    node->next = NULL;

    return node;
}

static void free_node(JQNode **node)
{
    free(*node);
    *node = NULL;
}
