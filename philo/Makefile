NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
#CFLAGS = -Wall -Wextra -Werror -fsanitize=thread -g

SRCS = main.c init.c philo.c utils.c philo_checks.c monitor.c
INCS = philo.h
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -pthread -o $(NAME) $(OBJS) -I $(INCS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
