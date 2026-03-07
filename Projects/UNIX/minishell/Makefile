# ============================================================
#  Makefile - minishell (42)
# ============================================================
#
#  Estructura:
#    srcs/         → codigo fuente organizado por modulos
#    includes/     → headers
#    libft/        → libreria de utilidades
#
#  El Makefile compila primero libft, luego minishell.
# ============================================================

NAME    = minishell

CC      = cc
CFLAGS  = -Wall -Wextra -Werror
INCLUDES = -I includes -I libft

# ─── Fuentes por modulo ──────────────────────────────────────
SRCS    = srcs/main.c \
          srcs/env/env_init.c \
          srcs/env/env_ops.c \
          srcs/env/env_utils.c \
          srcs/lexer/lexer.c \
          srcs/lexer/lexer_utils.c \
          srcs/parser/parser.c \
          srcs/parser/expander.c \
          srcs/exec/executor.c \
          srcs/exec/exec_utils.c \
          srcs/exec/redirections.c \
          srcs/builtins/builtins.c \
          srcs/builtins/builtin_echo.c \
          srcs/builtins/builtin_cd.c \
          srcs/builtins/builtin_env.c \
          srcs/builtins/builtin_exit.c \
          srcs/signals/signals.c

OBJS    = $(SRCS:.c=.o)

LIBFT   = libft/libft.a

# ─── Regla principal ─────────────────────────────────────────
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C libft

%.o: %.c includes/minishell.h libft/libft.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ─── Limpieza ────────────────────────────────────────────────
clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
