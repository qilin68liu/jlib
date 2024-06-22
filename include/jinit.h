#ifndef _J_INIT_H
#define _J_INIT_H

#include "jarray.h"

typedef JArray JInit;

// Constructor and Destructor
JInit *j_init_new(char *file);
void   j_init_free(JInit *init);

// Basic Operations
int j_init_get_integer_property(JInit *init, char *section, char *property, int *num);
int j_init_get_decimal_property(JInit *init, char *section, char *property, double *num);
int j_init_get_string_property(JInit *init, char *section, char *property, char **str);

#endif //!_J_INIT_H
