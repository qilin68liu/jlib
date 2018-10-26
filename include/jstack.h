#ifndef _J_STACK_H
#define _J_STACK_H

#include <stddef.h>
#include "jtypes.h"

typedef struct _j_stack JStack;

// Constructor and Destructor
JStack *j_stack_new();
void    j_stack_free(JStack *stack);
void    j_stack_free_deep(JStack *stack, JFreeFunc func);

// Basic Operations
int     j_stack_push(JStack *stack, void *data);
void   *j_stack_pop(JStack *stack);

// Stack Properties
size_t  j_stack_length(JStack *stack);
int     j_stack_empty(JStack *stack);
void    *j_stack_top(JStack *stack);

#endif // !_J_STACK_H
