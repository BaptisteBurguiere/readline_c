#include "readline_c_utils.h"

bool init_terminal(void)
{
    char *term_type = getenv("TERM");

    if (!term_type)
    {
        fprintf(stderr, "readline error: TERM must be set (see 'env').\n");
        return false;
    }

    switch (tgetent(NULL, term_type))
    {
        case -1:
            fprintf(stderr, "readline error: could not access to the termcap database..\n");
            return false;

        case 0:
            fprintf(stderr, "readline error: terminal type '%s' is not defined in termcap database (or have too few informations).\n", term_type);
            return false;
        
        default:
            break;
    }
    return true;
}

input init_input(void)
{
    input input;

    input.input = calloc(INPUT_SIZE, sizeof(char));
    input.index = 0;
    input.size = INPUT_SIZE;

    return input;
}

void increase_input(input *input)
{
    char *buf = input->input;
    input->input = calloc(input->size + INPUT_SIZE, sizeof(char));
    input->size += INPUT_SIZE;

    for (size_t i = 0; buf[i]; i++)
        input->input[i] = buf[i];

    free(buf);
}

void add_char(input *input, char c)
{
    if (strlen(input->input) >= input->size - 1)
        increase_input(input);

    if (input->index == strlen(input->input))
    {
        input->input[input->index] = c;
        printf("%c", c);
    }
    else
    {
        for (size_t i = strlen(input->input); i > input->index; i--)
            input->input[i] = input->input[i - 1];
        
        input->input[input->index] = c;
        char *cap = tgetstr("im", NULL);
        tputs(cap, 1, putchar);
        printf("%c", c);
        cap = tgetstr("ei", NULL);
        tputs(cap, 1, putchar);
    }
    input->index += 1;
    fflush(stdout);
}

void remove_char(input *input, char *prompt)
{
    if (input->index == 0)
        return;

    if (input->index == strlen(input->input))
        input->input[input->index - 1] = 0;
    else
    {
        for (size_t i = input->index; i < strlen(input->input) - 1; i++)
            input->input[i] = input->input[i + 1];
        
        input->input[input->index] = 0;
    }
    move_left(input, prompt);
    char *cap = tgetstr("dc", NULL);
    tputs(cap, 1, putchar);
    fflush(stdout);
}

void move_left(input *input, char *prompt)
{
    if (input->index > 0)
    {
        int nb_col = tgetnum("co");
        if ((input->index + strlen(prompt)) % nb_col == 0)
        {
            char *cap = tgetstr("up", NULL);
            tputs(cap, 1, putchar);
            cap = tgetstr("cr", NULL);
            tputs(cap, 1, putchar);
            cap = tgetstr("nd", NULL);
            for (int i = 0; i < nb_col; i++)
                tputs(cap, 1, putchar);
        }
        else
        {
            char *cap = tgetstr("le", NULL);
            tputs(cap, 1, putchar);
        }
        fflush(stdout);
        input->index -= 1;
    }
}

void move_right(input *input)
{
    if (input->index < strlen(input->input))
    {
        char *cap = tgetstr("nd", NULL);
        tputs(cap, 1, putchar);
        fflush(stdout);
        input->index += 1;
    }
}

void del_input(input *input, char *prompt)
{
    for (size_t i = input->index; i > 0; i--)
        remove_char(input, prompt);
}

void history_prev(input *input, char *prompt, History *history)
{
    if (history->nb_history < 1 || history->index < 1)
        return;
    
    del_input(input, prompt);
    history->index -= 1;
    for (size_t i = 0; i < strlen(history->history[history->index]); i++)
        add_char(input, history->history[history->index][i]);    
}

void history_next(input *input, char *prompt, History *history)
{
    if (history->nb_history < 1 || history->index >= history->nb_history)
        return;

    del_input(input, prompt);
    history->index += 1;
    if (history->index >= history->nb_history)
        return;
    for (size_t i = 0; i < strlen(history->history[history->index]); i++)
        add_char(input, history->history[history->index][i]);
}