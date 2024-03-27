#ifndef READLINE_C_UTILS_H
# define READLINE_C_UTILS_H

# include "readline_c.h"

# define INPUT_SIZE 64

typedef struct input
{
    char    *input;
    size_t  index;
    size_t  size;
}   input;

typedef struct History
{
    char    **history;
    size_t  nb_history;
    size_t  index;
}   History;

bool    init_terminal(void);
input   init_input(void);
void    increase_input(input *input);
void    add_char(input *input, char c);
void    remove_char(input *input, char *prompt);
void    move_left(input *input, char *prompt);
void    move_right(input *input);
void    history_prev(input *input, char *prompt, History *history);
void    history_next(input *input, char *prompt, History *history);
void    del_input(input *input, char *prompt);
void    auto_complete(input *input, char *prompt);

#endif