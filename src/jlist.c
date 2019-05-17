#include <stdlib.h>
#include "jlist.h"

struct _j_list {
    size_t length;
    size_t capacity;
    void **data;
};

JList *j_list_new()
{
    JList *list = (JList *)malloc(sizeof(JList));

    list->length = 0;
    list->capacity = 1;
    list->data = (void **)malloc(list->capacity * sizeof(void *));

    return list;
}

void j_list_free(JList *list)
{
    if(list == NULL)
        return;

    free(list->data);
    free(list);
}

void j_list_free_deep(JList *list, JFreeFunc func)
{
    if(list == NULL || func == NULL)
        return;

    for(size_t i = 0; i < list->length; i++)
        func(list->data[i]);

    free(list->data);
    free(list);
}

int j_list_add(JList *list, void *data)
{
    if(list == NULL)
        return 0;

    // if list buff is full, realloc the buff
    if(list->length == list->capacity)
    {
        list->capacity *= 2;
        list->data = (void **)realloc(list->data, list->capacity * sizeof(void *));
    }

    list->data[list->length++] = data;
    return 1;
}

void *j_list_nth(JList *list, size_t n)
{
    if(list == NULL || n < 0 || n >= list->length)
        return NULL;

    return list->data[n];
}

void *j_list_remove_nth(JList *list, size_t n)
{
    if(list == NULL || n < 0 || n >= list->length)
        return NULL;

    void *data = list->data[n];
    list->data[n] = NULL;

    for(size_t i = n; i < list->length - 1; i++)
        list->data[i] = list->data[i + 1];

    --list->length;
    return data;
}

JList *j_list_remove_if(JList *list, JPredicateFunc func, void *user_data)
{
    if(list == NULL || func == NULL)
        return NULL;

    int first = -1; // the first NULL in list
    JList *removed = j_list_new();

    for(size_t i = 0; i < list->length; i++)
    {
        if(func(list->data[i], user_data))
        {
            if(first == -1)
                first = i;
            j_list_add(removed, list->data[i]);
            list->data[i] = NULL;
        }
    }

    if(removed->length == 0)
    {
        j_list_free(removed);
        return NULL;
    }

    // since some data has been removed, we need to reorder the list
    for(size_t i = first + 1; i < list->length; i++)
    {
        if(list->data[i] != NULL)
        {
            list->data[first++] = list->data[i];
            list->data[i] = NULL;
        }
    }
    list->length -= removed->length;

    return removed;
}

int j_list_remove_deep_if(JList *list, JPredicateFunc pfunc, void *user_data, JFreeFunc ffunc)
{
    if(list == NULL || pfunc == NULL || ffunc == NULL)
        return 0;

    int first = -1; // the first NULL in list
    size_t count = 0;

    for(size_t i = 0; i < list->length; i++)
    {
        if(pfunc(list->data[i], user_data))
        {
            if(first == -1)
                first = i;
            ffunc(list->data[i]);
            list->data[i] = NULL;
            ++count;
        }
    }

    // since some data has been removed, we need to reorder the list
    if(first != -1)
    {
        for(size_t i = first + 1; i < list->length; i++)
        {
            if(list->data[i] != NULL)
            {
                list->data[first++] = list->data[i];
                list->data[i] = NULL;
            }
        }
    }

    list->length -= count;

    return count;
}

void j_list_foreach(JList *list, JFunc func, void *user_data)
{
    if(list == NULL || func == NULL)
        return;

    for(size_t i = 0; i < list->length; i++)
        func(list->data[i], user_data);
}

void *j_list_search(JList *list, JCompareFunc func, void *data)
{
    if(list == NULL || func == NULL)
        return NULL;

    for(size_t i = 0; i < list->length; i++)
        if(func(list->data[i], data))
            return list->data[i];

    return NULL;
}

int j_list_empty(JList *list)
{
    if(list == NULL || list->length == 0)
        return 1;

    return 0;
}

size_t j_list_length(JList *list)
{
    if(list == NULL)
        return 0;

    return list->length;
}

void *j_list_head(JList *list)
{
    if(list == NULL || list->length == 0)
        return NULL;

    return list->data[0];
}

void *j_list_tail(JList *list)
{
    if(list == NULL || list->length == 0)
        return NULL;

    return list->data[list->length - 1];
}

JList *j_list_copy(JList *list)
{
    if(list == NULL)
        return NULL;

    JList *new_list = j_list_new();

    for(size_t i = 0; i < list->length; i++)
        j_list_add(new_list, list->data[i]);

    return new_list;
}

JList *j_list_copy_deep(JList *list, JCopyFunc func)
{
    if(list == NULL || func == NULL)
        return NULL;

    JList *new_list = j_list_new();

    for(size_t i = 0; i < list->length; i++)
    {
        void *data = func(list->data[i]);
        j_list_add(new_list, data);
    }

    return new_list;
}
