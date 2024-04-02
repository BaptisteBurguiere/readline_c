# Readline for C

Returns input from the user.
Support for auto completion with TAB and history with up and down arrows.

DO NOT SUPPORT left and right arrows yet

## Installation

At the root of your project:

```bash
git clone git@github.com:BaptisteBurguiere/readline_c.git
```

Go in the parser directory and Make:

```bash
cd readline_c
make
```

## Include the library to your project

Add flags to the compilation:

```bash
<CC> -Ireadline_c -Lreadline_c <SRCS> -o <OUTPUT_FILE> -lreadline_c
```

Add the header in your file:

```c
#include <readline_c.h>
```

## Usage

Functions description:

```c
// Main function: take a prompt as parameter and returns the user input
char        *readline(char *prompt);

// Add a string to the history
void        add_history(char *str);

// Free history
void        destroy_history(void);
```

## Example

```c
#include <readline_c.h>

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
```
