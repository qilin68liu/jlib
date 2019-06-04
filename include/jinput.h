#ifndef _J_INPUT_H
#define _J_INPUT_H

char  *j_input_string(char *prompt);
int    j_input_integer(char *prompt);
double j_input_decimal(char *prompt);
char   j_input_single_choice(char *candidates, char *prompt);
char  *j_input_multiple_choice(char *candidates, char *prompt);

#endif // !_J_INPUT_H
