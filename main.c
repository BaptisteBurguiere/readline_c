#include "readline_c.h"

int main()
{
    char prompt[] = ">>> ";

    while (true)
    {
        char *output = readline(prompt);
        if (strcmp(output, "exit") == 0)
        {
            free(output);
            break;
        }
        if (strlen(output) > 0)
            add_history(output);
        free(output);
    }
    destroy_history();
}