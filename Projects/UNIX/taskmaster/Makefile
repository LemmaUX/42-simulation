# --- Taskmaster -------------------------------------------------------------
# Job control daemon (42 project "taskmaster")
# Inspired by supervisor. See README.md for usage.

NAME	= taskmaster

CC	= gcc
CFLAGS	= -Wall -Wextra -Werror -I includes -g -std=c11
LDFLAGS	=

SRCS_DIR	= srcs
OBJS_DIR	= objs

SRCS	= $(wildcard $(SRCS_DIR)/*.c)
OBJS	= $(patsubst $(SRCS_DIR)/%.c,$(OBJS_DIR)/%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo ">>> $(NAME) compiled."

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

clean:
	@rm -rf $(OBJS_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
