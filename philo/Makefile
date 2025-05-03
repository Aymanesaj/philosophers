CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC= helpers.c init.c philo.c
OBJ= $(SRC:.c=.o)
PHILO= philo


all: $(PHILO)

$(PHILO) : $(OBJ)
	$(CC) $(CFLAGS) -g -fsanitize=thread $(OBJ) -o $(PHILO)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(PHILO)

re: fclean all