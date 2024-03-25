#include "readline_c_utils.h"

char    **history = NULL;
size_t  nb_history = 0;

char *readline(char *prompt)
{
    static bool first = true;

    if (first)
    {
        char *term_type = getenv("TERM");

        if (!term_type)
        {
            fprintf(stderr, "readline error: TERM must be set (see 'env').\n");
            return NULL;
        }

        switch (tgetent(NULL, term_type))
        {
            case -1:
                fprintf(stderr, "readline error: could not access to the termcap database..\n");
                return NULL;

            case 0:
                fprintf(stderr, "readline error: terminal type '%s' is not defined in termcap database (or have too few informations).\n", term_type);
                return NULL;
            
            default:
                break;
        }

        first = false;
    }
    printf("%s", prompt);

    // int c = getch();

    // printf("%d\n", c);
    return NULL;
}

void add_history(char *str)
{
    if (!history)
        history = calloc(2, sizeof(char *));
    else
        history = realloc(history, nb_history + 2);

    history[nb_history] = strdup(str);
    nb_history++;
}

void destroy_history(void)
{
    for (size_t i = 0; i < nb_history; i++)
        free(history[i]);
    free(history);
}