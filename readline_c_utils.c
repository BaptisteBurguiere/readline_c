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
        input->index += 1;
    }
    else
    {
        for (size_t i = strlen(input->input); i > input->index; i--)
            input->input[i] = input->input[i - 1];
        
        input->input[input->index] = c;
    }
}

void remove_char(input *input)
{
    if (input->index == strlen(input->input))
    {
        input->input[input->index] = 0;
        input->index -= 1;
    }
    else
    {
        for (size_t i = input->index; i < strlen(input->input) - 1; i++)
            input->input[i] = input->input[i + 1];
        
        input->input[input->index] = 0;
    }
}

void refresh_line(input *input, char *prompt, size_t len)
{
    for (size_t i = len; i > 0; i--)
        printf("\b ");
    
    printf("%s%s", prompt, input->input);
    fflush(stdout);
}