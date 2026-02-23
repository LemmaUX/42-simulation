# ============================================================
#  Makefile - ft_printf (42)
# ============================================================
#
#  REGLAS REQUERIDAS POR EL SUBJECT:
#    all     → compila la librería libftprintf.a
#    clean   → elimina los archivos .o
#    fclean  → elimina .o + la librería
#    re      → fclean + all
#    bonus   → compila con archivos *_bonus.c (si los hubiera)
#
#  Por qué usamos 'ar' y no 'libtool':
#    El subject prohíbe libtool. 'ar rcs' crea el archivo .a
#    directamente: 'r'=insert, 'c'=create, 's'=add index.
#
#  Por qué no relinkeamos (regla del subject "must not relink"):
#    Make detecta automáticamente si los .o ya están al día.
#    Siempre que no modifiques los .c, no recompila.
# ============================================================

NAME    = libftprintf.a

CC      = cc
CFLAGS  = -Wall -Wextra -Werror

# Todos los archivos fuente del proyecto
SRCS    = ft_printf.c \
          handle_format.c \
          ft_putchar.c \
          ft_putstr.c \
          ft_putnbr.c \
          ft_putnbr_base.c \
          ft_putptr.c

# Regla de sustitución: cada .c genera su .o correspondiente
OBJS    = $(SRCS:.c=.o)

# ─── Regla principal ───────────────────────────────────────
all: $(NAME)

# Crea la librería estática a partir de los objetos compilados
$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

# Compila cada .c → .o (Make sabe que no hay que recompilar si el .o es más nuevo)
%.o: %.c ft_printf.h
	$(CC) $(CFLAGS) -c $< -o $@

# ─── Limpieza ──────────────────────────────────────────────
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# ─── Bonus (archivos *_bonus.c si los hay) ─────────────────
BONUS_SRCS  = $(wildcard *_bonus.c)
BONUS_OBJS  = $(BONUS_SRCS:.c=.o)

bonus: $(OBJS) $(BONUS_OBJS)
	ar rcs $(NAME) $(OBJS) $(BONUS_OBJS)

.PHONY: all clean fclean re bonus
