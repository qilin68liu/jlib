#ifndef _J_QUEUE_H
#define _J_QUEUE_H

#include "jtypes.h"

typedef struct _j_queue JQueue;

// Constructor and Destructor
JQueue *j_queue_new();
void    j_queue_free(JQueue **queue);
void    j_queue_free_deep(JQueue **queue, JFreeFunc func);

// Basic Operations
int     j_queue_enqueue(JQueue *queue, void *data);
void   *j_queue_dequeue(JQueue *queue);

// Queue Properties
int     j_queue_empty(JQueue *queue);
size_t  j_queue_length(JQueue *queue);
void   *j_queue_head(JQueue *queue);
void   *j_queue_tail(JQueue *queue);

#endif // !_J_QUEUE_H
