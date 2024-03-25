NAME=a.out
CC=gcc
FLAGS=-Wall -Wextra -Werror -g
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))

${NAME} : ${OBJS}
	$(CC) $(OBJS) -lcurses -o $(NAME)

.c.o :
	$(CC) ${FLAGS} -c $< -o ${<:.c=.o}

all : ${NAME}

clean :
	rm -rf ${OBJS}

fclean : clean
	rm -rf ${NAME}

re : fclean all