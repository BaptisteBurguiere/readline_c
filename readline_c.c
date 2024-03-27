#include "readline_c_utils.h"

History history = {NULL, 0, 0};

char *readline(char *prompt)
{
    static bool is_first = true;

    if (is_first)
    {
        if (!init_terminal())
            return NULL;
        is_first = false;
    }

    history.index = history.nb_history;
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
                            history_prev(&input, prompt, &history);
                            break;
                        case 'B': // Down arrow
                            history_next(&input, prompt, &history);
                            break;
                        // case 'C': // Right arrow
                        //     move_right(&input);
                        //     break;
                        // case 'D': // Left arrow
                        //     move_left(&input, prompt);
                        //     break;
                        default:
                            break;
                    }
                }
            }
        }
        else if (ch == 127) // Backspace
            remove_char(&input, prompt);
        else if (ch == '\t') // Tab
            auto_complete(&input, prompt);
        else if (isprint(ch)) // Printable character
            add_char(&input, ch);
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios); // Restore original terminal settings

    putchar('\n');
    return input.input;
}

void add_history(char *str)
{
    if (!history.history)
        history.history = calloc(2, sizeof(char *));
    else
        history.history= realloc(history.history, (history.nb_history + 2) * sizeof(char *));

    history.history[history.nb_history] = strdup(str);
    history.nb_history++;
}

void destroy_history(void)
{
    for (size_t i = 0; i < history.nb_history; i++)
        free(history.history[i]);
    free(history.history);
}