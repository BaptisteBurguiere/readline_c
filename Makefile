NAME = libreadline_c.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

CFILES = readline_c.c \
		readline_c_utils.c \

OFILES = $(CFILES:.c=.o)

$(NAME): $(OFILES)
	ar rcs $(NAME) $(OFILES)

all: $(NAME) clean

.c.o :
	$(CC) $(CFLAGS) -c $< -o ${<:.c=.o}

clean:
	rm -f $(OFILES)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY:  all clean fclean re