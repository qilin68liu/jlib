#ifndef _J_HASH_H
#define _J_HASH_H

#include <stddef.h>
#include "jtypes.h"

typedef struct _j_hash JHash;

JHash *j_hash_new(size_t length, JHashFunc hash_func);
void   j_hash_free(JHash **table);
void   j_hash_free_deep(JHash **table, JFreeFunc func);

size_t j_hash_size(JHash *table);

int j_hash_add(JHash *table, void *data);
int j_hash_remove(JHash *table, void *data);
int j_hash_remove_deep(JHash *table, void *data, JFreeFunc func);

void *j_hash_find(JHash *table, JCompareFunc func, void *data);
void  j_hash_foreach(JHash *table, JFunc func, void *user_data);

#endif // !_J_HASH_H
