#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "jtypes.h"
#include "jthreadpool.h"

static void print_num(void *data, void *user_data) {
    printf("%d\n", PTR_TO_INT(data));
}

int main(void) {
    JThreadPool *pool = j_threadpool_new(10);

    const size_t task_num = 10;
    JThreadPoolTask *tasks[task_num];

    for (size_t i = 0; i < task_num; ++i) {
        tasks[i] = j_threadpool_task_new(print_num, INT_TO_PTR(i), NULL, i);
        j_threadpool_add_task(pool, tasks[i]);
    }

    j_threadpool_shutdown(pool);
    j_threadpool_free(pool);

    for (size_t i = 0; i < task_num; ++i) {
        free(tasks[i]);
    }

    return 0;
}
