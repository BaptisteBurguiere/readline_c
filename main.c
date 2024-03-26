#include "readline_c.h"

int main()
{
    char prompt[] = ">>> ";

    while (true)
    {
        char *output = readline(prompt);
        if (strcmp(output, "exit\n") == 0)
        {
            free(output);
            break;
        }
        free(output);
    }
}