NAME = libgnl.a
TEST_NAME = gnl_test
CC = cc
CFLAGS = -Wall -Wextra -Werror
BUFFER_SIZE ?= 42

SRC = get_next_line.c get_next_line_utils.c
TEST_SRC = main.c
OBJ = $(SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)

all: $(NAME)

test: $(NAME) $(TEST_NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

$(TEST_NAME): $(TEST_OBJ) $(NAME)
	$(CC) $(CFLAGS) -D BUFFER_SIZE=$(BUFFER_SIZE) $(TEST_OBJ) -L. -lgnl -o $(TEST_NAME)

%.o: %.c get_next_line.h
	$(CC) $(CFLAGS) -D BUFFER_SIZE=$(BUFFER_SIZE) -c $< -o $@

clean:
	rm -f $(OBJ) $(TEST_OBJ)

fclean: clean
	rm -f $(NAME) $(TEST_NAME)

re: fclean all

.PHONY: all clean fclean re test
