#ifndef READLINE_C_H
# define READLINE_C_H

# include <stddef.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <term.h>
# include <stdio.h>

char        *readline(char *prompt);
void        add_history(char *str);
void        destroy_history(void);

#endif