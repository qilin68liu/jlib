#ifndef _J_ARRAY_H
#define _J_ARRAY_H

#include <stddef.h>
#include "jtypes.h"

typedef struct _j_array JArray;

// Custructor and Destructor
JArray *j_array_new();
void    j_array_free(JArray *array);
void    j_array_free_deep(JArray *array, JFreeFunc func);

// Basic Operations
int     j_array_add(JArray *array, void *data);
void   *j_array_get_nth(JArray *array, size_t n);
int     j_array_set_nth(JArray *array, size_t n, void *data);
int     j_array_insert_nth(JArray *array, size_t n, void *data);
void   *j_array_remove_nth(JArray *array, size_t n);
JArray *j_array_remove_if(JArray *array, JPredicateFunc func, void *user_data);
int     j_array_remove_deep_if(JArray *array, JPredicateFunc pfunc, void *user_data, JFreeFunc ffunc);
void    j_array_foreach(JArray *array, JFunc func, void *user_data);
void   *j_array_search(JArray *array, JPredicateFunc func, void *user_data);
int     j_array_sort(JArray *array, JCompareFunc func, void *data);

// Array Properties
int    j_array_empty(JArray *array);
size_t j_array_length(JArray *array);
void  *j_array_head(JArray *array);
void  *j_array_tail(JArray *array);

// Copy features
JArray *j_array_copy(JArray *array);
JArray *j_array_copy_deep(JArray *array, JCopyFunc func);

#endif // !_J_ARRAY_H
