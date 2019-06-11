#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jinit.h"
#include "jstring.h"
#include "jtypes.h"

#define BUFF_SIZE 128

#define CANNOT_FIND     "Error: Cannot Find Property"
#define NO_SECTION      "Error: No Section Found"
#define INVALID_FILE    "Error: Cannot Open File"
#define INVALID_FORMAT  "Error: Invalid Config Format"
#define INVALID_LINE    "Error: Section/Property/Comment Too Long"
#define INVALID_VALUE   "Error: Invalid Value"
#define PROPERTY_EXIST  "Error: Property Already Exist"
#define SECTION_EXIST   "Error: Section Already Exist"

static char BUFF[BUFF_SIZE];

typedef struct _section {
    char *name;
    JList *properties;
} Section;

typedef struct _property {
    char *name;
    char *value;
} Property;

typedef enum _line_type {
    EMPTY,
    COMMENT,
    PAIR,
    SECTION,
    INVALID
} LineType;

static Section *section_new(char *name);
static void section_free(Section *section);

static Property *property_new(char *name, char *value);
static void property_free(Property *property);

static int valid_section(char *str);
static int valid_property(char *str);
static int valid_comment(char *str);

static LineType check_type(char *str);

static Section *make_section(char *str);
static Property *make_property(char *str);

static int search_section(Section *section, char *name);
static int search_property(Property *property, char *name);

static char *try_get_value(JInit *init, char *section, char *property);

JInit *j_init_new(char *file)
{
    FILE *f = fopen(file, "r");
    if(f == NULL)
    {
        fprintf(stderr, "%s %s\n", INVALID_FILE, file);
        return NULL;
    }

    int i = 0;
    char ch;
    JList *list = j_list_new();
    Section *sec = NULL;
    Property *property = NULL;
    unsigned line_num = 0;

    while((ch = fgetc(f)) != EOF)
    {
        if(i < BUFF_SIZE - 1 && ch != '\n')
            BUFF[i++] = ch;
        else if(ch == '\n')
        {
            BUFF[i] = '\0';
            ++line_num;

            LineType type = check_type(BUFF);
            switch(type)
            {
                case EMPTY:
                case COMMENT:
                    // do nothing:
                    break;
                case PAIR:
                    if(j_list_tail(list) == NULL)
                    {
                        fprintf(stderr, "%s (at line %u)\n", NO_SECTION, line_num);
                        goto error;
                    }
                    property = make_property(BUFF);
                    if(!j_list_search(((Section *)j_list_tail(list))->properties, (JCompareFunc)search_property, property->name))
                        j_list_add(((Section *)j_list_tail(list))->properties, property);
                    else
                    {
                        fprintf(stderr, "%s (at line %u)\n", PROPERTY_EXIST, line_num);
                        property_free(property);
                        goto error;
                    }
                    break;
                case SECTION:
                    sec = make_section(BUFF);
                    if(!j_list_search(list, (JCompareFunc)search_section, sec->name))
                        j_list_add(list, sec);
                    else
                    {
                        fprintf(stderr, "%s (at line %u)\n", SECTION_EXIST, line_num);
                        section_free(sec);
                        goto error;
                    }
                    break;
                case INVALID:
                    fprintf(stderr, "%s (at line %u)\n", INVALID_FORMAT, line_num);
                default:
                error: // explicit lable used for goto
                    j_list_free_deep(list, (JFreeFunc)section_free);
                    fclose(f);
                    return NULL;
            }

            i = 0;
            continue;
        }
        else if(i >= BUFF_SIZE - 1)
        {
            fprintf(stderr, "%s (at line %u)\n", INVALID_LINE, line_num + 1);
            goto error;
        }
    }

    fclose(f);
    return (JInit *)list;
}

void j_init_free(JInit *init)
{
    j_list_free_deep((JList *)init, (JFreeFunc)section_free);
}

int j_init_get_integer_property(JInit *init, char *section, char *property, int *num)
{
    char *value = try_get_value(init, section, property);

    if(value == NULL)
        return 0;

    if(!j_is_integer(value))
    {
        fprintf(stderr, "%s: %s, Expect Integer.\n", INVALID_VALUE, value);
        return 0;
    }

    *num = atoi(value);
    return 1;
}

int j_init_get_decimal_property(JInit *init, char *section, char *property, double *num)
{
    char *value = try_get_value(init, section, property);

    if(value == NULL)
        return 0;

    if(!j_is_decimal(value))
    {
        fprintf(stderr, "%s: %s, Expect Decimal.\n", INVALID_VALUE, value);
        return 0;
    }

    *num = strtod(value, NULL);
    return 1;
}

int j_init_get_string_property(JInit *init, char *section, char *property, char **str)
{
    char *value = try_get_value(init, section, property);

    if(value == NULL)
        return 0;

    char *new_str = (char *)malloc(strlen(value) + 1);
    strcpy(new_str, value);
    *str = new_str;
    return 1;
}

static Section *section_new(char *name)
{
    Section *sec = (Section *)malloc(sizeof(Section));
    sec->name = name;
    sec->properties = j_list_new();

    return sec;
}

static void section_free(Section *section)
{
    free(section->name);
    j_list_free_deep(section->properties, (JFreeFunc)property_free);
    free(section);
}

static Property *property_new(char *name, char *value)
{
    Property *p = (Property *)malloc(sizeof(Property));
    p->name = name;
    p->value = value;

    return p;
}

static void property_free(Property *property)
{
    free(property->name);
    free(property->value);
    free(property);
}

static int valid_section(char *str)
{
    // first & last character should be [ & ]
    if(str[0] != '[' || str[strlen(str) - 1] != ']')
        return 0;

    // should only contain one [ & ]
    int l = 0, r = 0;
    while(*str != '\0')
    {
        if(*str == '[')
            l++;
        if(*str == ']')
            r++;
        ++str;
    }

    if(l != 1 || r != 1)
        return 0;

    return 1;
}

static int valid_property(char *str)
{
    // should contain one and only one '='
    int e = 0;
    char *s = str;
    while(*s != '\0')
    {
        if(*(s++) == '=')
            e++;
    }

    if(e != 1)
        return 0;

    return 1;
}

static int valid_comment(char *str)
{
    if(str[0] == '#')
        return 1;

    return 0;
}

static LineType check_type(char *str)
{
    if(strlen(str) == 0)
        return EMPTY;

    if(strchr(str, '=') != NULL && valid_property(str))
        return PAIR;

    if(strchr(str, '[') != NULL && valid_section(str))
        return SECTION;

    if(strchr(str, '#') != NULL && valid_comment(str))
        return COMMENT;

    return INVALID;
}

static Section *make_section(char *str)
{
    char *start = strchr(str, '[') + 1;
    char *end = (strchr(str, ']'));
    *end = '\0';

    char *name = (char *)malloc(strlen(start) + 1);
    strcpy(name, start);

    return section_new(name);
}

static Property *make_property(char *str)
{
    JList *list = j_string_split(str, "=");
    char *property = j_string_trim(j_list_get_nth(list, 0));
    char *value = j_string_trim(j_list_get_nth(list, 1));
    j_list_free_deep(list, free);

    return property_new(property, value);
}

static int search_section(Section *section, char *name)
{
    if(strcmp(section->name, name) == 0)
        return 1;

    return 0;
}

static int search_property(Property *property, char *name)
{
    if(strcmp(property->name, name) == 0)
        return 1;

    return 0;
}

static char *try_get_value(JInit *init, char *section, char *property)
{
    if(init == NULL || section == NULL || property == NULL)
        return NULL;

    Section *sec = j_list_search((JList *)init, (JCompareFunc)search_section, section);
    if(sec == NULL)
    {
        fprintf(stderr, "%s [%s]->[%s]\n", CANNOT_FIND, section, property);
        return NULL;
    }

    Property *p = j_list_search(sec->properties, (JCompareFunc)search_property, property);
    if(p == NULL)
    {
        fprintf(stderr, "%s [%s]->[%s]\n", CANNOT_FIND, section, property);
        return NULL;
    }

    return p->value;
}
