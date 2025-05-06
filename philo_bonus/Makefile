CC = cc
CFLAGS = -Wall -Wextra -Werror #-g -fsanitize=thread
SRC= helpers.c init.c philo.c
OBJ= $(SRC:.c=.o)
PHILO= philo


all: $(PHILO)

$(PHILO) : $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(PHILO)

%.o: %.c philo.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(PHILO)

re: fclean all