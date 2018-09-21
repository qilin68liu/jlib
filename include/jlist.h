#ifndef _J_LIST_H
#define _J_LIST_H

#include "jtypes.h"

typedef struct _j_list JList;

// Custructor and Destructor
JList *j_list_new();
void   j_list_free(JList **list);
void   j_list_free_deep(JList **list, JFreeFunc func);

// Basic Operations
int    j_list_add(JList *list, void *data);
int    j_list_remove(JList *list, void *data);
int    j_list_remove_deep(JList *list, void *data, JFreeFunc func);
JList *j_list_remove_if(JList *list, JPredicateFunc func, void *user_data);
int    j_list_remove_deep_if(JList *list, JPredicateFunc pfunc, void *user_data, JFreeFunc ffunc);
void   j_list_foreach(const JList *list, JFunc func, void *user_data);
void  *j_list_find(const JList *list, JCompareFunc func, const void *data);

// List Properties
int    j_list_empty(const JList *list);
size_t j_list_length(const JList *list);
void  *j_list_head(const JList *list);
void  *j_list_tail(const JList *list);

// Copy features
JList *j_list_copy(const JList *list);
JList *j_list_copy_deep(const JList *list, JCopyFunc func);

#endif // !_J_LIST_H
