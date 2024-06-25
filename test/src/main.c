#include <stdio.h>
#include <stdlib.h>

#include "jtypes.h"
#include "jprequeue.h"

int main(void) {
    JPreQueue *prequeue = j_prequeue_new(j_ptr_gt);

    const int max = 100000;
    for (int i = 1; i <= max; ++i)
        j_prequeue_push(prequeue, INT_TO_PTR(i));

    for (int i = max; i >= 1; --i) {
        void *data = j_prequeue_pop(prequeue);
        if (PTR_TO_INT(data) != i) {
            printf("error\n");
        }
    }

    printf("pass\n");
    j_prequeue_free(prequeue);

    return 0;
}
