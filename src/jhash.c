#include <stdlib.h>
#include "jhash.h"

struct _j_hash {
    size_t length;
    size_t size;
    JArray **table;
    JHashFunc hash_func;
};

static void merge_to(void *data, void *user_data);

JHash *j_hash_new(size_t length, JHashFunc hash_func) {
    JHash *table = (JHash *)malloc(sizeof(JHash));

    table->length = length;
    table->size = 0;
    table->table = (JArray **)malloc(length * sizeof(JArray *));
    for(int i = 0; i < length; i++) {
        table->table[i] = j_array_new();
    }

    table->hash_func = hash_func;

    return table;
}

void j_hash_free(JHash *table) {
    if (table == NULL) {
        return;
    }

    // free the arrays
    for(int i = 0; i < table->length; i++) {
        j_array_free(table->table[i]);
    }
    free(table->table);

    free(table);
}

void j_hash_free_deep(JHash *table, JFreeFunc func) {
    if (table == NULL || func == NULL) {
        return;
    }

    // free the arrays
    for(int i = 0; i < table->length; i++) {
        j_array_free_deep(table->table[i], func);
    }
    free(table->table);

    free(table);
}

size_t j_hash_size(JHash *table) {
    return (table == NULL ? 0 : table->size);
}

int j_hash_add(JHash *table, void *data) {
    if (table == NULL) {
        return 0;
    }

    j_array_add(table->table[table->hash_func(data)], data);
    table->size++;

    return 1;
}

JArray *j_hash_remove_if(JHash *table, JPredicateFunc func, void *user_data) {
    if (table == NULL || func == NULL) {
        return NULL;
    }

    JArray *removed = j_array_new();
    JArray *tmp = NULL;
    for(int i = 0; i < table->length; i++) {
        tmp = j_array_remove_if(table->table[i], func, user_data);
        j_array_foreach(tmp, merge_to, removed);
        j_array_free(tmp);
    }

    if (j_array_length(removed) == 0) {
        j_array_free(removed);
        return NULL;
    }

    table->size -= j_array_length(removed);

    return removed;
}

int j_hash_remove_deep_if(JHash *table, JPredicateFunc pfunc, void *user_data, JFreeFunc ffunc) {
    if (table == NULL || pfunc == NULL || ffunc == NULL) {
        return 0;
    }

    int count = 0;
    for(int i = 0; i < table->length; i++) {
        count += j_array_remove_deep_if(table->table[i], pfunc, user_data, ffunc);
    }
    table->size -= count;

    return count;
}

void *j_hash_search(JHash *table, JPredicateFunc func, void *user_data) {
    if (table == NULL || func == NULL) {
        return NULL;
    }

    return j_array_search(table->table[table->hash_func(user_data)], func, user_data);
}

void j_hash_foreach(JHash *table, JFunc func, void *user_data) {
    if (table == NULL || func == NULL) {
        return;
    }

    for(int i = 0; i < table->length; i++) {
        j_array_foreach(table->table[i], func, user_data);
    }
}

static void merge_to(void *data, void *user_data) {
    j_array_add((JArray *)user_data, data);
}
