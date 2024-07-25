#ifndef _J_PREQUEUE_H
#define _J_PREQUEUE_H

#include <stddef.h>
#include "jtypes.h"

typedef struct _j_prequeue JPreQueue;

// Constructor and Destructor
JPreQueue *j_prequeue_new(JCompareFunc func);
void       j_prequeue_free(JPreQueue *prequeue);
void       j_prequeue_free_deep(JPreQueue *prequeue, JFreeFunc free_func);

// Basic Operations
void *j_prequeue_pop(JPreQueue *prequeue);
int   j_prequeue_push(JPreQueue *prequeue, void *data);

// JPreQueue Properties
size_t  j_prequeue_length(JPreQueue *prequeue);
int     j_prequeue_empty(JPreQueue *prequeue);
void   *j_prequeue_top(JPreQueue *prequeue);

#endif
