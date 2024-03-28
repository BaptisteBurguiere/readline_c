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

char **split_dir_file(char *input)
{
    char **dir_file = calloc(2, sizeof(char *));
    dir_file[0] = calloc(strlen(input) + 2, sizeof(char));
    dir_file[1] = calloc(strlen(input) + 2, sizeof(char));
    int end = (int)strlen(input) - 1;

    while (end >= 0 && input[end] != '/')
        end--;

    if (end < 0)
    {
        dir_file[0][0] = '.';
        free(dir_file[1]);
        dir_file[1] = strdup(input);
    }
    else
    {
        for (size_t i = 0; i <= (size_t)end; i++)
            dir_file[0][i] = input[i];
        for (size_t i = (size_t)end + 1; i < strlen(input); i++)
            dir_file[1][i - (end + 1)] = input[i];
    }

    return dir_file;
}

void auto_complete(input *input, char *prompt)
{
    (void)prompt;
    char **dir_file = split_dir_file(input->input);
    DIR *dir = opendir(dir_file[0]);
    if (!dir)
    {
        free(dir_file[0]);
        free(dir_file[1]);
        free(dir_file);
        return;
    }
    
    struct dirent *dirent;
    size_t nb_files = 0;
    while ((dirent = readdir(dir)) != NULL)
    {
        if (strcmp(dirent->d_name, ".") == 0
            || strcmp(dirent->d_name, "..") == 0)
            continue;

        if (strncmp(dir_file[1], dirent->d_name, strlen(dir_file[1])) == 0)
            nb_files++;
    }
    closedir(dir);

    if (nb_files == 1)
    {
        dir = opendir(dir_file[0]);
        while ((dirent = readdir(dir)) != NULL)
        {
            if (strcmp(dirent->d_name, ".") == 0
                || strcmp(dirent->d_name, "..") == 0)
                continue;

            if (strncmp(dir_file[1], dirent->d_name, strlen(dir_file[1])) == 0)
            {
                for (size_t i = strlen(dir_file[1]); i < strlen(dirent->d_name); i++)
                    add_char(input, dirent->d_name[i]);
                if (dirent->d_type == 4)
                    add_char(input, '/');
                break;
            }
        }
        closedir(dir);
    }
    else
    {
        dir = opendir(dir_file[0]);
        char **files = calloc(nb_files, sizeof(char *));
        size_t i = 0;
        while ((dirent = readdir(dir)) != NULL)
        {
            if (strcmp(dirent->d_name, ".") == 0
                || strcmp(dirent->d_name, "..") == 0)
                continue;

            if (strncmp(dir_file[1], dirent->d_name, strlen(dir_file[1])) == 0)
            {
                files[i] = strdup(dirent->d_name);
                i++;
                if (i == nb_files)
                    break;
            }
        }
        closedir(dir);

        bool stop = false;
        for (i = strlen(dir_file[1]); files[0][i]; i++)
        {
            char c = files[0][i];
            for (size_t j = 1; j < nb_files; j++)
            {
                if (files[j][i] != c)
                {
                    stop = true;
                    break;
                }
            }
            if (stop)
                break;
            add_char(input, c);
        }

        for (i = 0; i < nb_files; i++)
            free(files[i]);
        free(files);
    }
    free(dir_file[0]);
    free(dir_file[1]);
    free(dir_file);
}