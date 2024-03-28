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
# include <dirent.h>

char        *readline(char *prompt);
void        add_history(char *str);
void        destroy_history(void);

#endif