#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jtypes.h>
#include "jinput.h"

#define BUFF_SIZE 128
#define INVALID_MSG "Invalid input.\n"

static char BUFF[BUFF_SIZE];

char *j_read_string(const char *prompt)
{
    int i = 0;
    char ch;

    if(prompt != NULL)
        printf("%s", prompt);
    while(i < BUFF_SIZE - 1 && (ch = getchar()) != '\n')
        BUFF[i++] = ch;
    BUFF[i] = '\0';

    if(i == 0)
        return NULL;

    char *str = (char *)malloc(i + 1);
    strcpy(str, BUFF);

    return str;
}

int j_read_integer(const char *prompt)
{
    int i;
    char ch;

    while(1)
    {
        i = 0;

        if(prompt != NULL)
            printf("%s", prompt);
        while(i < BUFF_SIZE - 1 && (ch = getchar()) != '\n')
            BUFF[i++] = ch;
        BUFF[i] = '\0';

        if(j_is_integer(BUFF))
            break;
        else
            fprintf(stderr, INVALID_MSG);
    }

    return atoi(BUFF);
}

double j_read_double(const char *prompt)
{
    int i;
    char ch;

    while(1)
    {
        i = 0;

        if(prompt != NULL)
            printf("%s", prompt);
        while(i < BUFF_SIZE - 1 && (ch = getchar()) != '\n')
            BUFF[i++] = ch;
        BUFF[i] = '\0';

        if(j_is_float(BUFF))
            break;
        else
            fprintf(stderr, INVALID_MSG);
    }

    return strtod(BUFF, NULL);
}

char j_read_single_choice(const char *candidates, const char *prompt)
{
    int i;
    char ch;

    while(1)
    {
        i = 0;

        if(prompt != NULL)
            printf("%s", prompt);
        while(i < BUFF_SIZE - 1 && (ch = getchar()) != '\n')
            BUFF[i++] = ch;
        BUFF[i] = '\0';

        if(strlen(BUFF) == 1 && strchr(candidates, BUFF[0]) != NULL)
            break;
        else
            fprintf(stderr, INVALID_MSG);
    }

    return BUFF[0];
}

char *j_read_multiple_choice(const char *candidates, const char *prompt)
{
    int i;
    char ch;

    while(1)
    {
        i = 0;

        if(prompt != NULL)
            printf("%s", prompt);
        while(i < BUFF_SIZE - 1 && (ch = getchar()) != '\n')
            BUFF[i++] = ch;
        BUFF[i] = '\0';

        int all_in = 1;
        for(int j = 0; j < i; j++)
        {
            if(strchr(candidates, BUFF[j]) == NULL)
            {
                all_in = 0;
                break;
            }
        }

        if(all_in)
            break;
        else
            fprintf(stderr, INVALID_MSG);
    }

    char *str = (char *)malloc(i + 1);
    strcpy(str, BUFF);

    return str;
}
