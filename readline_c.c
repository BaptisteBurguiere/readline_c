#include "readline_c_utils.h"

char    **history = NULL;
size_t  nb_history = 0;

char *readline(char *prompt)
{
    printf("%s", prompt);
    fflush(stdout);

    input input = init_input();
    char ch;

    struct termios original_termios, new_termios;
    tcgetattr(STDIN_FILENO, &original_termios);
    new_termios = original_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    while (read(STDIN_FILENO, &ch, 1) == 1 && ch != '\n') {
        if (ch == 27) { // Escape sequence, possibly an arrow key
            // Read the next two characters to distinguish arrow keys
            if (read(STDIN_FILENO, &ch, 1) == 1 && ch == '[') {
                if (read(STDIN_FILENO, &ch, 1) == 1) {
                    // Handle arrow keys
                    switch (ch) {
                        case 'A': // Up arrow
                            // Implement your logic here
                            break;
                        case 'B': // Down arrow
                            // Implement your logic here
                            break;
                        case 'C': // Right arrow
                            // Implement your logic here
                            break;
                        case 'D': // Left arrow
                            printf("\b");
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        else if (ch == 127) // Backspace
        {
            if (input.index > 0)
            {
                remove_char(&input);
                refresh_line(&input, prompt, strlen(prompt) + strlen(input.input) + 1);
            }
        }
        else if (isprint(ch)) // Printable character
        {
            add_char(&input, ch);
            refresh_line(&input, prompt, strlen(prompt) + strlen(input.input));
        }
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios); // Restore original terminal settings

    putchar('\n');
    return input.input; // Return input as a dynamically allocated string
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