#ifndef _J_TYPES_H_
#define _J_TYPES_H_

#include <stddef.h>

#define J_SHORT(X)  (*(short *)X)
#define J_INT(X)    (*(int *)X)
#define J_LONG(X)   (*(long *)X)
#define J_FLOAT(X)  (*(float *)X)
#define J_DOUBLE(X) (*(double *)X)
#define J_USHORT(X) (*(unsigned short *)X)
#define J_UINT(X)   (*(unsigned int *)X)
#define J_ULONG(X)  (*(unsigned long *)X)

typedef void  (*JFunc)          (void *data, void *user_data);
typedef int   (*JCompareFunc)   (const void *a, const void *b);
typedef int   (*JPredicateFunc) (const void *data, void *user_data);
typedef void  (*JFreeFunc)      (void **data);
typedef void *(*JCopyFunc)      (const void *data);

// Define some basic data creation for convenience
short  *j_short_new(short num);
int    *j_int_new(int num);
long   *j_long_new(long num);
float  *j_float_new(float num);
double *j_double_new(double num);

unsigned short  *j_ushort_new(unsigned short num);
unsigned int    *j_uint_new(unsigned int num);
unsigned long   *j_ulong_new(unsigned long num);

void j_atom_free(void **data);

#endif // !_J_TYPES_H_
