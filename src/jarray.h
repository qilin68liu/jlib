#include <stdlib.h>
#include "jarray.h"

struct _j_array {
    size_t length;
    size_t capacity;
    void **data;
};

static void sort_array(void **data, int start, int end, JCompareFunc func);
static int sort_helper(void **data, int start, int end, JCompareFunc func);

JArray *j_array_new() {
    JArray *array = (JArray *)malloc(sizeof(JArray));

    array->length = 0;
    array->capacity = 1;
    array->data = (void **)malloc(array->capacity * sizeof(void *));

    return array;
}

void j_array_free(JArray *array) {
    if(array == NULL)
        return;

    free(array->data);
    free(array);
}

void j_array_free_deep(JArray *array, JFreeFunc func) {
    if(array == NULL || func == NULL)
        return;

    for(size_t i = 0; i < array->length; i++)
        func(array->data[i]);

    free(array->data);
    free(array);
}

int j_array_add(JArray *array, void *data) {
    if(array == NULL)
        return 0;

    // if array buff is full, realloc the buff
    if(array->length == array->capacity) {
        array->capacity *= 2;
        array->data = (void **)realloc(array->data, array->capacity * sizeof(void *));
    }

    array->data[array->length++] = data;
    return 1;
}

void *j_array_get_nth(JArray *array, size_t n) {
    if(array == NULL || n < 0 || n >= array->length)
        return NULL;

    return array->data[n];
}

int j_array_set_nth(JArray *array, size_t n, void *data) {
    if(array == NULL || n < 0 || n >= array->length)
        return 0;

    array->data[n] = data;
    return 1;
}

int j_array_insert_nth(JArray *array, size_t n, void *data) {
    if(array == NULL || n < 0 || n >= array->length)
        return 0;

    if(array->length == array->capacity) {
        array->capacity *= 2;
        array->data = (void *)realloc(array->data, array->capacity * sizeof(void *));
    }

    for(size_t i = array->length - 1; i >= n; i--)
        array->data[i + 1] = array->data[i];
    array->data[n] = data;

    ++array->length;
    return 1;
}

void *j_array_remove_nth(JArray *array, size_t n) {
    if(array == NULL || n < 0 || n >= array->length)
        return NULL;

    void *data = array->data[n];
    array->data[n] = NULL;

    for(size_t i = n; i < array->length - 1; i++)
        array->data[i] = array->data[i + 1];

    --array->length;
    return data;
}

JArray *j_array_remove_if(JArray *array, JPredicateFunc func, void *user_data) {
    if(array == NULL || func == NULL)
        return NULL;

    int first = -1; // the first NULL in array
    JArray *removed = j_array_new();

    for(size_t i = 0; i < array->length; i++) {
        if(func(array->data[i], user_data)) {
            if(first == -1)
                first = i;
            j_array_add(removed, array->data[i]);
            array->data[i] = NULL;
        }
    }

    if(removed->length == 0) {
        j_array_free(removed);
        return NULL;
    }

    // since some data has been removed, we need to reorder the array
    for(size_t i = first + 1; i < array->length; i++) {
        if(array->data[i] != NULL) {
            array->data[first++] = array->data[i];
            array->data[i] = NULL;
        }
    }
    array->length -= removed->length;

    return removed;
}

int j_array_remove_deep_if(JArray *array, JPredicateFunc pfunc, void *user_data, JFreeFunc ffunc) {
    if(array == NULL || pfunc == NULL || ffunc == NULL)
        return 0;

    int first = -1; // the first NULL in array
    size_t count = 0;

    for(size_t i = 0; i < array->length; i++) {
        if(pfunc(array->data[i], user_data)) {
            if(first == -1)
                first = i;
            ffunc(array->data[i]);
            array->data[i] = NULL;
            ++count;
        }
    }

    // since some data has been removed, we need to reorder the array
    if(first != -1) {
        for(size_t i = first + 1; i < array->length; i++) {
            if(array->data[i] != NULL) {
                array->data[first++] = array->data[i];
                array->data[i] = NULL;
            }
        }
    }

    array->length -= count;

    return count;
}

void j_array_foreach(JArray *array, JFunc func, void *user_data) {
    if(array == NULL || func == NULL)
        return;

    for(size_t i = 0; i < array->length; i++)
        func(array->data[i], user_data);
}

void *j_array_search(JArray *array, JPredicateFunc func, void *user_data) {
    if(array == NULL || func == NULL)
        return NULL;

    for(size_t i = 0; i < array->length; i++)
        if(func(array->data[i], user_data))
            return array->data[i];

    return NULL;
}

int j_array_sort(JArray *array, JCompareFunc func, void *user_data) {
    if(array == NULL || func == NULL)
        return 0;

    sort_array(array->data, 0, array->length - 1, func);

    return 1;
}

int j_array_empty(JArray *array) {
    if(array == NULL || array->length == 0)
        return 1;

    return 0;
}

size_t j_array_length(JArray *array) {
    if(array == NULL)
        return 0;

    return array->length;
}

void *j_array_head(JArray *array) {
    if(array == NULL || array->length == 0)
        return NULL;

    return array->data[0];
}

void *j_array_tail(JArray *array) {
    if(array == NULL || array->length == 0)
        return NULL;

    return array->data[array->length - 1];
}

JArray *j_array_copy(JArray *array) {
    if(array == NULL)
        return NULL;

    JArray *new_array = j_array_new();

    for(size_t i = 0; i < array->length; i++)
        j_array_add(new_array, array->data[i]);

    return new_array;
}

JArray *j_array_copy_deep(JArray *array, JCopyFunc func) {
    if(array == NULL || func == NULL)
        return NULL;

    JArray *new_array = j_array_new();

    for(size_t i = 0; i < array->length; i++) {
        void *data = func(array->data[i]);
        j_array_add(new_array, data);
    }

    return new_array;
}

static void sort_array(void **data, int start, int end, JCompareFunc func) {
    if(start < end) {
        int mid = sort_helper(data, start, end, func);
        sort_array(data, start, mid - 1, func);
        sort_array(data, mid + 1, end, func);
    }
}

static int sort_helper(void **data, int start, int end, JCompareFunc func) {
    void *key = data[end];
    void *tmp = NULL;

    int i = start - 1;
    for(int j = start; j < end; j++) {
        if(func(data[j], key)) {
            ++i;
            tmp = data[i];
            data[i] = data[j];
            data[j] = tmp;
        }
    }

    tmp = data[i + 1];
    data[i + 1] = data[end];
    data[end] = tmp;

    return i + 1;
}
