#ifndef _J_TYPES_H_
#define _J_TYPES_H_

#define J_SHORT(X)  (*(short *)(X))
#define J_INT(X)    (*(int *)(X))
#define J_LONG(X)   (*(long *)(X))
#define J_FLOAT(X)  (*(float *)(X))
#define J_DOUBLE(X) (*(double *)(X))
#define J_USHORT(X) (*(unsigned short *)(X))
#define J_UINT(X)   (*(unsigned int *)(X))
#define J_ULONG(X)  (*(unsigned long *)(X))

#define INT_TO_PTR(X) ((void *)(long)(X))
#define PTR_TO_INT(X) ((int)(long)(X))

typedef void  (*JFunc)             (void *data, void *user_data);
typedef int   (*JCompareFunc)      (void *a, void *b);
typedef int   (*JPredicateFunc)    (void *data, void *user_data);
typedef void  (*JFreeFunc)         (void *data);
typedef void *(*JCopyFunc)         (void *data);
typedef void  (*JEventHandlerFunc) (void *object, void *user_data);
typedef int   (*JHashFunc)         (void *data);

// Define some basic data creation for convenience
int    *j_int_new(int num);
int    *j_int_copy(void *data);
double *j_double_new(double num);
double *j_double_copy(void *data);

int j_int_cm(void *a, void *b);
int j_int_eq(void *a, void *b);
int j_int_lt(void *a, void *b);
int j_int_le(void *a, void *b);
int j_int_gt(void *a, void *b);
int j_int_ge(void *a, void *b);

int j_double_cm(void *a, void *b);
int j_double_eq(void *a, void *b);
int j_double_lt(void *a, void *b);
int j_double_le(void *a, void *b);
int j_double_gt(void *a, void *b);
int j_double_ge(void *a, void *b);

int j_is_integer(char *str);
int j_is_decimal(char *str);

#endif // !_J_TYPES_H_
