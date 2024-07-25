#ifndef J_THREADPOOL_H
#define J_THREADPOOL_H

#include <stddef.h>
#include "jtypes.h"

typedef struct _j_threadpool_task   JThreadPoolTask;
typedef struct _j_threadpool        JThreadPool;

JThreadPoolTask *j_threadpool_task_new(JFunc func, void *data, void *user_data, int priority);

// Constructor and Destructor
JThreadPool *j_threadpool_new(size_t thread_num);
void         j_threadpool_free(JThreadPool *pool);

void j_threadpool_shutdown(JThreadPool *pool);
void j_threadpool_add_task(JThreadPool *pool, JThreadPoolTask *task);

#endif
