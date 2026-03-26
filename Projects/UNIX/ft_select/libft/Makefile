NAME = libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs

SRCS = ft_strlen.c \
	   ft_strcpy.c \
	   ft_strdup.c \
	   ft_memcpy.c \
	   ft_memset.c \
	   ft_strcmp.c \
	   ft_strcat.c \
	   ft_putchar.c \
	   ft_putstr.c \
	   ft_atoi.c \
	   ft_itoa.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
