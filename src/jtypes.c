#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "jtypes.h"

int *j_int_new(int num) {
    int *n = (int *)malloc(sizeof(int));
    *n = num;
    return n;
}

int *j_int_copy(void *data) {
    return j_int_new(J_INT(data));
}

double *j_double_new(double num) {
    double *n = (double *)malloc(sizeof(double));
    *n = num;
    return n;
}

double *j_double_copy(void *data) {
    return j_double_new(J_DOUBLE(data));
}

int j_int_cm(void *a, void *b) {
    return J_INT(a) - J_INT(b);
}

int j_int_eq(void *a, void *b) {
    return (J_INT(a) == J_INT(b) ? 1 : 0);
}

int j_int_lt(void *a, void *b) {
    return (J_INT(a) < J_INT(b) ? 1 : 0);
}

int j_int_le(void *a, void *b) {
    return (J_INT(a) <= J_INT(b) ? 1 : 0);
}

int j_int_gt(void *a, void *b) {
    return (J_INT(a) > J_INT(b) ? 1 : 0);
}

int j_int_ge(void *a, void *b) {
    return (J_INT(a) >= J_INT(b) ? 1 : 0);
}

int j_ptr_cm(void *a, void *b) {
    return (a - b);
}

int j_ptr_eq(void *a, void *b) {
    return (a == b ? 1 : 0);
}

int j_ptr_lt(void *a, void *b) {
    return (a < b ? 1 : 0);
}

int j_ptr_le(void *a, void *b) {
    return (a <= b ? 1 : 0);
}

int j_ptr_gt(void *a, void *b) {
    return (a > b ? 1 : 0);
}

int j_ptr_ge(void *a, void *b) {
    return (a >= b ? 1 : 0);
}

int j_double_cm(void *a, void *b) {
    return J_DOUBLE(a) - J_DOUBLE(b);
}

int j_double_eq(void *a, void *b) {
    return (J_DOUBLE(a) == J_DOUBLE(b) ? 1 : 0);
}

int j_double_lt(void *a, void *b) {
    return (J_DOUBLE(a) < J_DOUBLE(b) ? 1 : 0);
}

int j_double_le(void *a, void *b) {
    return (J_DOUBLE(a) <= J_DOUBLE(b) ? 1 : 0);
}

int j_double_gt(void *a, void *b) {
    return (J_DOUBLE(a) > J_DOUBLE(b) ? 1 : 0);
}

int j_double_ge(void *a, void *b) {
    return (J_DOUBLE(a) >= J_DOUBLE(b) ? 1 : 0);
}

int j_is_integer(char *str) {
    if (str == NULL) {
        return 0;
    }

    // First char should be +,- or digit.
    if (*str != '+' && *str != '-' && !isdigit(*str)) {
        return 0;
    }

    while (*(++str) != '\0') {
        if (!isdigit(*str)) {
            return 0;
        }
    }

    return 1;
}

int j_is_decimal(char *str) {
    if (str == NULL) {
        return 0;
    }

    // First char should be +/-/. or digit.
    if (*str != '+' && *str != '-' && *str != '.' && !isdigit(*str)) {
        return 0;
    }

    int found = 0;
    if (*str == '.') {
        found = 1;
    }
    while (*(++str) != '\0') {
        if (*str != '.' && !isdigit(*str)) {
            return 0;
        }
        // float should contain only one digit
        if (*str == '.') {
            if (found) {
                return 0;
            } else {
                found = 1;
            }
        }
    }

    return 1;
}
