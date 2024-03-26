#ifndef READLINE_C_UTILS_H
# define READLINE_C_UTILS_H

# include "readline_c.h"

bool    init_terminal(void);
input   init_input(void);
void    increase_input(input *input);
void    add_char(input *input, char c);
void    remove_char(input *input);
void    refresh_line(input *input, char *prompt, size_t len);

#endif