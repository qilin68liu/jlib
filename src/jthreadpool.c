#include "jthreadpool.h"
#include "jprequeue.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef void (*ThreadPoolTaskFunc)(void *, void *);

struct _j_threadpool_task {
    JThreadPoolTaskFunc func;
    int                 priority;
    void               *data;
};

struct _j_threadpool {
    JPreQueue          *prequeue;
    int                 shutdown;
    pthread_cond_t      not_empty;
    pthread_mutex_t     locker;
    pthread_t          *threads;
    size_t              thread_num;
};

static int task_compare(void *a, void *b) {
    return ((JThreadPoolTask *)a)->priority < ((JThreadPoolTask *)b)->priority;
}

static void *thread_func(void *data) {
    JThreadPool *pool = data;

    while (1) {
        pthread_mutex_lock(&pool->locker);
        while (!pool->shutdown && j_prequeue_empty(pool->prequeue)) {
            pthread_cond_wait(&pool->not_empty, &pool->locker);
        }

        if (pool->shutdown && j_prequeue_empty(pool->prequeue)) {
            pthread_mutex_unlock(&pool->locker);
            break;
        }

        // take first task
        JThreadPoolTask *task = j_prequeue_pop(pool->prequeue);
        pthread_mutex_unlock(&pool->locker);

        // run task
        task->func(task->data);

        // free task
        free(task);
    }

    return NULL;
}

JThreadPoolTask *j_threadpool_task_new(JThreadPoolTaskFunc func, void *data, int priority) {
    JThreadPoolTask *task = (JThreadPoolTask *)malloc(sizeof(JThreadPoolTask));

    task->func = func;
    task->data = data;
    task->priority = priority;

    return task;
}

JThreadPool *j_threadpool_new(size_t thread_num) {
    JThreadPool *pool = (JThreadPool *)malloc(sizeof(JThreadPool));

    pool->prequeue = j_prequeue_new(task_compare);
    pthread_cond_init(&pool->not_empty, NULL);
    pthread_mutex_init(&pool->locker, NULL);
    pool->threads = (pthread_t *)malloc(thread_num * sizeof(pthread_t));
    pool->thread_num = thread_num;
    pool->shutdown = 0;

    for (size_t i = 0; i < thread_num; ++i) {
        pthread_create(&pool->threads[i], NULL, thread_func, pool);
    }

    return pool;
}

void j_threadpool_free(JThreadPool *pool) {
    j_prequeue_free(pool->prequeue);
    pthread_cond_destroy(&pool->not_empty);
    pthread_mutex_destroy(&pool->locker);
    free(pool->threads);
    free(pool);
}

void j_threadpool_shutdown(JThreadPool *pool) {
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->not_empty);
    for (size_t i = 0; i < pool->thread_num; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
}

void j_threadpool_add_task(JThreadPool *pool, JThreadPoolTask *task) {
    pthread_mutex_lock(&pool->locker);
    if (!pool->shutdown) {
        j_prequeue_push(pool->prequeue, task);
    }
    pthread_mutex_unlock(&pool->locker);
    pthread_cond_signal(&pool->not_empty);
}
