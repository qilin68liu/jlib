#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jtypes.h"
#include "jinput.h"

#define BUFF_SIZE 128
#define INVALID_MSG "Invalid input.\n"

static char BUFF[BUFF_SIZE];

char *j_input_string(char *prompt)
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

int j_input_integer(char *prompt)
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

double j_input_decimal(char *prompt)
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

        if(j_is_decimal(BUFF))
            break;
        else
            fprintf(stderr, INVALID_MSG);
    }

    return strtod(BUFF, NULL);
}

char j_input_single_choice(char *candidates, char *prompt)
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

char *j_input_multiple_choice(char *candidates, char *prompt)
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
