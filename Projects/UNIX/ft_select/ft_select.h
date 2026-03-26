#ifndef FT_SELECT_H
# define FT_SELECT_H

# include <stdlib.h>
# include <unistd.h>
# include <termios.h>
# include <term.h>
# include <sys/ioctl.h>
# include <signal.h>
# include <fcntl.h>
# include "libft/libft.h"

# define KEY_UP 1000
# define KEY_DOWN 1001
# define KEY_LEFT 1002
# define KEY_RIGHT 1003
# define KEY_ESCAPE 1004
# define KEY_SPACE 1005
# define KEY_RETURN 1006
# define KEY_DELETE 1007
# define KEY_BACKSPACE 1008
# define KEY_SIGWINCH 1009

typedef struct s_item
{
	char			*str;
	int				selected;
	int				deleted;
	struct s_item	*next;
	struct s_item	*prev;
}	t_item;

typedef struct s_display
{
	int				max_cols;
	int				max_rows;
	int				items_per_col;
	int				cols_needed;
	int				col_width;
	int				total_items;
	int				deleted_count;
}	t_display;

typedef struct s_select
{
	t_item			*items;
	t_item			*current;
	t_display		display;
	struct termios	old_term;
	struct termios	new_term;
	int				running;
	int				cols;
	int				rows;
}	t_select;

extern t_select	*g_select;

void			setup_terminal(void);
void			restore_terminal(void);
void			setup_signals(void);
void			signal_handler(int sig);

t_item			*create_item(char *str);
void			add_item(t_item **items, t_item *new_item);
void			delete_item(t_select *select, t_item *item);
void			free_items(t_item *items);

int				read_key(void);
void			handle_key(t_select *select, int key);

void			display_list(t_select *select);
void			clear_display(t_select *select);
void			move_cursor(int x, int y);

int				get_terminal_size(int *cols, int *rows);
void			calculate_layout(t_select *select);

void			render_items(t_select *select);

#endif
