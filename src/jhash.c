#include <stdlib.h>
#include "jhash.h"
#include "jlist.h"

struct _j_hash {
    size_t length;
    size_t size;
    JList **table;
    JHashFunc hash_func;
};

JHash *j_hash_new(size_t length, JHashFunc hash_func)
{
    JHash *table = (JHash *)malloc(sizeof(JHash));

    table->length = length;
    table->size = 0;
    table->table = (JList **)malloc(length * sizeof(JList *));
    for(int i = 0; i < length; i++)
        table->table[i] = j_list_new();

    table->hash_func = hash_func;

    return table;
}

void j_hash_free(JHash **table)
{
    if(*table == NULL)
        return;

    // free the lists
    for(int i = 0; i < (*table)->length; i++)
        j_list_free(&(*table)->table[i]);
    free((*table)->table);

    free(*table);
    *table = NULL;
}

void j_hash_free_deep(JHash **table, JFreeFunc func)
{
    if(*table == NULL || func == NULL)
        return;

    // free the lists
    for(int i = 0; i < (*table)->length; i++)
        j_list_free_deep(&(*table)->table[i], func);
    free((*table)->table);

    free(*table);
    *table = NULL;
}

size_t j_hash_size(JHash *table)
{
    if(table == NULL)
        return 0;

    return table->size;
}

int j_hash_add(JHash *table, void *data)
{
    if(table == NULL)
        return 0;

    j_list_add(table->table[table->hash_func(data)], data);
    table->size++;

    return 1;
}

int j_hash_remove(JHash *table, void *data)
{
    if(table == NULL)
        return 0;

    j_list_remove(table->table[table->hash_func(data)], data);
    table->size--;

    return 1;
}

int j_hash_remove_deep(JHash *table, void *data, JFreeFunc func)
{
    if(table == NULL || func == NULL)
        return 0;

    j_list_remove_deep(table->table[table->hash_func(data)], data, func);
    table->size--;

    return 1;
}

void *j_hash_find(JHash *table, JCompareFunc func, void *data)
{
    if(table == NULL || func == NULL)
        return NULL;

    return j_list_find(table->table[table->hash_func(data)], func, data);
}

void j_hash_foreach(JHash *table, JFunc func, void *user_data)
{
    if(table == NULL || func == NULL)
        return;

    for(int i = 0; i < table->length; i++)
        j_list_foreach(table->table[i], func, user_data);
}
