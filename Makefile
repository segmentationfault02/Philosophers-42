NAME = philo
CC = cc
CFLAGS = -g -Wall -Wextra -Werror -I./includes

SRCS = main.c \
	   src/dinner.c \
	   src/getters_setters.c \
	   src/init.c \
	   src/monitor.c \
	   src/parsing.c \
	   src/safe_functions.c \
	   src/synchro_utils.c \
	   src/utils.c

OBJS = $(SRCS:.c=.o)

GREEN = \033[0;32m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(LIBFT) -lpthread -o $(NAME)
	@echo "$(GREEN)* * * * Philosophers compilation done with no errors! * * * *$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN)* * * * Philosophers directory successfully cleaned! * * * *$(RESET)"

re: fclean all

.PHONY: all clean fclean re
