# ============================================================
#  Makefile - libft para minishell
# ============================================================

NAME    = libft.a

CC      = cc
CFLAGS  = -Wall -Wextra -Werror

SRCS    = ft_is.c \
          ft_str.c \
          ft_strdup.c \
          ft_split.c \
          ft_mem.c \
          ft_convert.c \
          ft_put.c

OBJS    = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c libft.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
