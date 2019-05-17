#ifndef _J_STRING_H
#define _J_STRING_H

#include "jlist.h"

char  *j_string_trim(char *str);
JList *j_string_split(char *str, char *delimeter);

#endif //!_J_STRING_H
