NAME = bfi
SRC = bf_interpreter.c actions.c
CC = gcc
INCLUDE = ./
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDE)

all: $(NAME)

$(NAME):
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm -rf $(NAME)

.PHONY: all clean
