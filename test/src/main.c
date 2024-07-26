#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "jtypes.h"
#include "jthreadpool.h"

static void print_num(void *data) {
    usleep(20000);
    printf("%ld\n", PTR_TO_SIZE(data));
}

int main(void) {
    JThreadPool *pool = j_threadpool_new(10);

    const size_t task_num = 100;
    for (size_t i = 0; i < task_num; ++i) {
        j_threadpool_add_task(pool, print_num, SIZE_TO_PTR(i), i);
    }

    j_threadpool_shutdown(pool);
    j_threadpool_free(pool);

    return 0;
}
