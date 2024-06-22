#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "jstring.h"

char *j_string_trim(char *str) {
    if(str == NULL)
        return NULL;

    size_t s = 0;
    while(isspace(str[s]))
        ++s;

    size_t e = strlen(str) - 1;
    while(isspace(str[e]))
        --e;

    size_t len = e - s + 1;
    char *new_str = (char *)malloc(len + 1);
    memcpy(new_str, str + s, len);
    new_str[len] = '\0';

    return new_str;
}

JArray *j_string_split(char *str, char *delimeter) {
    if(str == NULL || delimeter == NULL)
        return NULL;

    size_t s = 0, e = 0;
    JArray *array = j_array_new();

    while(e <= strlen(str)) {
        if(strchr(delimeter, str[e]) != NULL || str[e] == '\0') {
            size_t len = e - s;
            char *tmp = (char *)malloc(len + 1);
            memcpy(tmp, str + s, len);
            tmp[len] = '\0';
            j_array_add(array, tmp);
            s = e + 1;
        }
        ++e;
    }

    return array;
}
