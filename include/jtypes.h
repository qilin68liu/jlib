#ifndef _J_TYPES_H_
#define _J_TYPES_H_

#include <stddef.h>

typedef int   (*JPredicateFunc) (const void *a, const void *b);
typedef int   (*JCompareFunc)   (const void *a, const void *b);
typedef void  (*JFunc)          (void *data, void *user_data);
typedef void  (*JFreeFunc)      (void **data);
typedef void *(*JCopyFunc)      (const void *data);

#endif // !_J_TYPES_H_
