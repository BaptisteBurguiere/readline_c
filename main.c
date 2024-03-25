#include "readline_c.h"

int main()
{
    char prompt[] = ">>> ";

    char *output = readline(prompt);

    // printf("%s\n", output);
    free(output);
}