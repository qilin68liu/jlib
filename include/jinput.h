#ifndef _J_INPUT_H
#define _J_INPUT_H

char  *j_read_string(const char *prompt);
int    j_read_integer(const char *prompt);
double j_read_double(const char *prompt);
char   j_read_single_choice(const char *candidates, const char *prompt);
char  *j_read_multiple_choice(const char *candidates, const char *prompt);

#endif // !_J_INPUT_H
