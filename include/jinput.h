#ifndef _J_INPUT_H
#define _J_INPUT_H

char  *j_read_string(char *prompt);
int    j_read_integer(char *prompt);
double j_read_double(char *prompt);
char   j_read_single_choice(char *candidates, char *prompt);
char  *j_read_multiple_choice(char *candidates, char *prompt);

#endif // !_J_INPUT_H
