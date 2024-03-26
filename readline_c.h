#ifndef READLINE_C_H
# define READLINE_C_H

# include <stddef.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <term.h>
# include <termios.h>
# include <stdio.h>
# include <sys/select.h>
# include <unistd.h>
# include <ctype.h>

# define INPUT_SIZE 64

typedef struct input
{
    char    *input;
    size_t  index;
    size_t  size;
}   input;

char        *readline(char *prompt);
void        add_history(char *str);
void        destroy_history(void);

#endif