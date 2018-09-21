#include <stddef.h>
#include <stdlib.h>
#include "jlist.h"

typedef struct _j_lnode JLNode;

struct _j_lnode {
    void *data;
    JLNode *prev;
    JLNode *next;
};

struct _j_list {
    size_t length;
    JLNode *head;
    JLNode *tail;
};

static JLNode *new_node(void *data);
static void free_node(JLNode **node);
static void remove_node(JList *list, JLNode *node);

JList *j_list_new()
{
    JList *list = (JList *)malloc(sizeof(JList));

    list->length = 0;
    list->head = list->tail = NULL;

    return list;
}

void j_list_free(JList **list)
{
    if(*list == NULL)
        return;

    JLNode *cur = (*list)->head;
    JLNode *del = NULL;
    while(cur != NULL)
    {
        del = cur;
        cur = cur->next;
        free_node(&del);
    }

    free(*list);
    *list = NULL;
}

void j_list_free_deep(JList **list, JFreeFunc func)
{
    if(*list == NULL || func == NULL)
        return;

    JLNode *cur = (*list)->head;
    JLNode *del = NULL;
    while(cur != NULL)
    {
        del = cur;
        cur = cur->next;
        func(&(del->data));
        free_node(&del);
    }

    free(*list);
    *list = NULL;
}

int j_list_add(JList *list, void *data)
{
    if(list == NULL)
        return 0;

    JLNode *node = new_node(data);
    if(list->length == 0)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    list->length++;
    return 1;
}

int j_list_remove(JList *list, void *data)
{
    if(list == NULL || data == NULL)
        return 0;

    JLNode *cur = list->head;
    while(cur != NULL && cur->data != data)
    {
        cur = cur->next;
    }

    if(cur == NULL)
        return 0;

    remove_node(list, cur);

    return 1;
}

int j_list_remove_deep(JList *list, void *data, JFreeFunc func)
{
    if(list == NULL || data == NULL)
        return 0;

    JLNode *cur = list->head;
    while(cur != NULL && cur->data != data)
    {
        cur = cur->next;
    }

    if(cur == NULL)
        return 0;

    remove_node(list, cur);
    func(&data);

    return 1;
}

JList *j_list_remove_if(JList *list, JPredicateFunc func, void *user_data)
{
    if(list == NULL || func == NULL)
        return NULL;

    JList *removed = j_list_new();
    JLNode *cur = list->head;
    JLNode *del = NULL;
    while(cur != NULL)
    {
        if(func(cur->data, user_data))
        {
            del = cur;
            cur = cur->next;
            j_list_add(removed, del->data);
            remove_node(list, del);
        }
        else
        {
            cur = cur->next;
        }
    }

    if(removed->length == 0)
    {
        j_list_free(&removed);
        return NULL;
    }

    return removed;
}

int j_list_remove_deep_if(JList *list, JPredicateFunc pfunc, void *user_data, JFreeFunc ffunc)
{
    if(list == NULL || pfunc == NULL || ffunc == NULL)
        return 0;

    int count = 0;
    JLNode *cur = list->head;
    JLNode *del = NULL;
    while(cur != NULL)
    {
        if(pfunc(cur->data, user_data))
        {
            del = cur;
            cur = cur->next;
            ffunc(&(del->data));
            remove_node(list, del);
            count++;
        }
        else
        {
            cur = cur->next;
        }
    }

    return count;
}

void j_list_foreach(const JList *list, JFunc func, void *user_data)
{
    if(list == NULL || func == NULL)
        return;

    JLNode *cur = list->head;
    while(cur != NULL)
    {
        func(cur->data, user_data);
        cur = cur->next;
    }
}

void *j_list_find(const JList *list, JCompareFunc func, const void *data)
{
    if(list == NULL || func == NULL)
        return NULL;

    JLNode *cur = list->head;
    while(cur != NULL && func(cur->data, data) != 0)
        cur = cur->next;

    if(cur == NULL)
        return NULL;

    return cur->data;
}

int j_list_empty(const JList *list)
{
    if(list == NULL || list->length == 0)
        return 1;

    return 0;
}

size_t j_list_length(const JList *list)
{
    if(list == NULL)
        return 0;

    return list->length;
}

void *j_list_head(const JList *list)
{
    if(list == NULL)
        return NULL;

    return list->head->data;
}

void *j_list_tail(const JList *list)
{
    if(list == NULL)
        return NULL;

    return list->tail->data;
}

JList *j_list_copy(const JList *list)
{
    if(list == NULL)
        return NULL;

    JList *new_list = j_list_new();
    JLNode *cur = list->head;
    while(cur != NULL)
    {
        j_list_add(new_list, cur->data);
        cur = cur->next;
    }

    return new_list;
}

JList *j_list_copy_deep(const JList *list, JCopyFunc func)
{
    if(list == NULL || func == NULL)
        return NULL;

    JList *new_list = j_list_new();
    JLNode *cur = list->head;
    while(cur != NULL)
    {
        void *data = func(cur->data);
        j_list_add(new_list, data);
        cur = cur->next;
    }

    return new_list;
}

static JLNode *new_node(void *data)
{
    JLNode *node = (JLNode *)malloc(sizeof(JLNode));

    node->data = data;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

static void free_node(JLNode **node)
{
    if(*node == NULL)
        return;

    free(*node);
    *node = NULL;
}

static void remove_node(JList *list, JLNode *node)
{
    if(node == NULL)
        return;

    if(list->head == node && list->tail == node)
    {
        list->head = list->tail = NULL;
    }
    else if(list->head == node)
    {
        list->head = list->head->next;
        list->head->prev = NULL;
    }
    else if(list->tail == node)
    {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    free_node(&node);
    list->length--;
}
