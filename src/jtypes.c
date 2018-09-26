#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "jtypes.h"

short *j_short_new(short num)
{
    short *n = (short *)malloc(sizeof(short));
    *n = num;
    return n;
}

int *j_int_new(int num)
{
    int *n = (int *)malloc(sizeof(int));
    *n = num;
    return n;
}

long *j_long_new(long num)
{
    long *n = (long *)malloc(sizeof(long));
    *n = num;
    return n;
}

float *j_float_new(float num)
{
    float *n = (float *)malloc(sizeof(float));
    *n = num;
    return n;
}

double *j_double_new(double num)
{
    double *n = (double *)malloc(sizeof(double));
    *n = num;
    return n;
}

unsigned short *j_ushort_new(unsigned short num)
{
    unsigned short *n = (unsigned short *)malloc(sizeof(unsigned short));
    *n = num;
    return n;
}

unsigned int *j_uint_new(unsigned int num)
{
    unsigned int *n = (unsigned int *)malloc(sizeof(unsigned int));
    *n = num;
    return n;
}

unsigned long *j_ulong_new(unsigned long num)
{
    unsigned long *n = (unsigned long *)malloc(sizeof(unsigned long));
    *n = num;
    return n;
}

int j_is_integer(const char *str)
{
    if(str == NULL)
        return 0;

    // First char should be +,- or digit.
    if(*str != '+' && *str != '-' && !isdigit(*str))
        return 0;

    while(*(++str) != '\0')
        if(!isdigit(*str))
            return 0;

    return 1;
}

int j_is_float(const char *str)
{
    if(str == NULL)
        return 0;

    // First char should be +/-/. or digit.
    if(*str != '+' && *str != '-' && *str != '.' && !isdigit(*str))
        return 0;

    int found = 0;
    if(*str == '.')
        found = 1;
    while(*(++str) != '\0')
    {
        if(*str != '.' && !isdigit(*str))
            return 0;
        // float should contain only one digit
        if(*str == '.')
        {
            if(found)
                return 0;
            else
                found = 1;
        }
    }

    return 1;
}

void j_atom_free(void **data)
{
    free(*data);
    *data = NULL;
}
