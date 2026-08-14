/* ************************************************************************** */
/*                                                                            */
/*   termcaps.c - Line edition usando termcaps                                */
/*                                                                            */
/*   Implementa la edicion de linea con:                                      */
/*     - Movimiento del cursor (izquierda, derecha, inicio, fin)              */
/*     - Historial de comandos (arriba, abajo)                                */
/*     - Edicion de caracteres (insertar, borrar)                             */
/*     - Soporte para ctrl-C y ctrl-D                                         */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static char	*g_line_buffer = NULL;
static int	g_cursor_pos = 0;
static int	g_line_len = 0;
static int	g_reading = 0;

/*
** sigint_handler_line: Maneja ctrl-C durante la edicion de linea.
** Limpia la linea actual y fuerza un nuevo prompt.
*/
static void	sigint_handler_line(int sig)
{
	(void)sig;
	if (g_reading)
	{
		write(STDOUT_FILENO, "\n", 1);
		g_line_buffer = NULL;
	}
}

/*
** termcaps_init: Inicializa la biblioteca termcaps.
** Obtiene las capacidades del terminal y configura el modo raw.
** Retorna 0 si exito, -1 si falla.
*/
int	termcaps_init(t_termcaps *tc)
{
	char	*term;
	char	buf[1024];
	int		ret;

	term = getenv("TERM");
	if (!term)
		return (-1);
	ret = tgetent(buf, term);
	if (ret <= 0)
		return (-1);
	tc->cm = tgetstr("cm", NULL);
	tc->up = tgetstr("up", NULL);
	tc->down = tgetstr("do", NULL);
	tc->left = tgetstr("le", NULL);
	tc->right = tgetstr("nd", NULL);
	tc->home = tgetstr("ho", NULL);
	tc->end = tgetstr("cr", NULL);
	tc->cl = tgetstr("cl", NULL);
	tc->cd = tgetstr("cd", NULL);
	tc->ce = tgetstr("ce", NULL);
	tc->enabled = 1;
	tcgetattr(STDIN_FILENO, &tc->orig_termios);
	tc->new_termios = tc->orig_termios;
	tc->new_termios.c_lflag &= ~(ICANON | ECHO);
	tc->new_termios.c_cc[VMIN] = 1;
	tc->new_termios.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &tc->new_termios);
	return (0);
}

/*
** termcaps_restore: Restaura el terminal a su estado original.
*/
void	termcaps_restore(t_termcaps *tc)
{
	if (tc->enabled)
		tcsetattr(STDIN_FILENO, TCSANOW, &tc->orig_termios);
}

/*
** move_cursor: Mueve el cursor a una posicion relativa.
** delta > 0: derecha, delta < 0: izquierda
*/
static void	move_cursor(t_termcaps *tc, int delta)
{
	int	i;

	if (delta > 0)
	{
		i = 0;
		while (i < delta && tc->right)
		{
			tputs(tc->right, 1, ft_putchar_fd);
			i++;
		}
	}
	else if (delta < 0)
	{
		i = 0;
		while (i < -delta && tc->left)
		{
			tputs(tc->left, 1, ft_putchar_fd);
			i++;
		}
	}
}

/*
** clear_to_end: Limpia desde el cursor hasta el final de la linea.
*/
static void	clear_to_end(t_termcaps *tc)
{
	if (tc->ce)
		tputs(tc->ce, 1, ft_putchar_fd);
}

/*
** redraw_line: Redibuja la linea actual desde el principio.
*/
static void	redraw_line(t_termcaps *tc, char *line, int len, int cursor)
{
	int	i;

	if (tc->home)
		tputs(tc->home, 1, ft_putchar_fd);
	write(STDOUT_FILENO, line, len);
	clear_to_end(tc);
	if (tc->home)
		tputs(tc->home, 1, ft_putchar_fd);
	i = 0;
	while (i < cursor)
	{
		if (tc->right)
			tputs(tc->right, 1, ft_putchar_fd);
		i++;
	}
}

/*
** insert_char: Inserta un caracter en la posicion del cursor.
*/
static void	insert_char(char **line, int *len, int *cursor, char c)
{
	char	*new;
	int		i;

	new = malloc(*len + 2);
	if (!new)
		return ;
	i = 0;
	while (i < *cursor)
	{
		new[i] = (*line)[i];
		i++;
	}
	new[i] = c;
	while (i < *len)
	{
		new[i + 1] = (*line)[i];
		i++;
	}
	new[*len + 1] = '\0';
	free(*line);
	*line = new;
	(*len)++;
	(*cursor)++;
}

/*
** delete_char: Borra el caracter antes del cursor (backspace).
*/
static void	delete_char(char **line, int *len, int *cursor)
{
	char	*new;
	int		i;

	if (*cursor <= 0 || *len <= 0)
		return ;
	new = malloc(*len);
	if (!new)
		return ;
	i = 0;
	while (i < *cursor - 1)
	{
		new[i] = (*line)[i];
		i++;
	}
	while (i < *len - 1)
	{
		new[i] = (*line)[i + 1];
		i++;
	}
	new[*len - 1] = '\0';
	free(*line);
	*line = new;
	(*len)--;
	(*cursor)--;
}

/*
** add_history: Agrega una linea al historial.
*/
void	add_history(t_shell *shell, const char *line)
{
	t_history	*new;
	t_history	*last;

	if (!line || ft_strlen(line) == 0)
		return ;
	new = malloc(sizeof(t_history));
	if (!new)
		return ;
	new->line = ft_strdup(line);
	new->next = NULL;
	if (!shell->history)
	{
		new->prev = NULL;
		shell->history = new;
	}
	else
	{
		last = shell->history;
		while (last->next)
			last = last->next;
		last->next = new;
		new->prev = last;
	}
	shell->hist_count++;
	if (shell->hist_count > HISTORY_SIZE)
	{
		last = shell->history;
		shell->history = last->next;
		if (shell->history)
			shell->history->prev = NULL;
		free(last->line);
		free(last);
		shell->hist_count--;
	}
	shell->hist_current = NULL;
}

/*
** history_up: Navega hacia atras en el historial.
*/
void	history_up(t_shell *shell)
{
	if (!shell->history)
		return ;
	if (!shell->hist_current)
		shell->hist_current = shell->history;
	else if (shell->hist_current->prev)
		shell->hist_current = shell->hist_current->prev;
	if (shell->hist_current && shell->hist_current->line)
	{
		free(shell->current_line);
		shell->current_line = ft_strdup(shell->hist_current->line);
		shell->line_len = ft_strlen(shell->current_line);
		shell->cursor_pos = shell->line_len;
	}
}

/*
** history_down: Navega hacia adelante en el historial.
*/
void	history_down(t_shell *shell)
{
	if (!shell->hist_current)
		return ;
	if (shell->hist_current->next)
		shell->hist_current = shell->hist_current->next;
	else
		shell->hist_current = NULL;
	if (shell->hist_current && shell->hist_current->line)
	{
		free(shell->current_line);
		shell->current_line = ft_strdup(shell->hist_current->line);
		shell->line_len = ft_strlen(shell->current_line);
		shell->cursor_pos = shell->line_len;
	}
	else
	{
		free(shell->current_line);
		shell->current_line = ft_strdup("");
		shell->line_len = 0;
		shell->cursor_pos = 0;
	}
}

/*
** read_line_edit: Lee una linea con edicion completa.
** Retorna la linea completada o NULL si EOF (ctrl-D).
*/
char	*read_line_edit(t_shell *shell)
{
	char	buf[6];
	int		ret;
	int		prompt_len;

	g_reading = 1;
	g_line_buffer = ft_strdup("");
	g_cursor_pos = 0;
	g_line_len = 0;
	shell->current_line = g_line_buffer;
	shell->cursor_pos = 0;
	shell->line_len = 0;
	signal(SIGINT, sigint_handler_line);
	prompt_len = ft_strlen(PROMPT);
	ft_putstr_fd(PROMPT, STDOUT_FILENO);
	while (g_reading)
	{
		ret = read(STDIN_FILENO, buf, 1);
		if (ret <= 0)
		{
			if (g_line_len == 0)
			{
				g_reading = 0;
				free(g_line_buffer);
				return (NULL);
			}
			break ;
		}
		if (buf[0] == 4) /* ctrl-D */
		{
			if (g_line_len == 0)
			{
				g_reading = 0;
				free(g_line_buffer);
				return (NULL);
			}
			continue ;
		}
		if (buf[0] == 3) /* ctrl-C */
		{
			write(STDOUT_FILENO, "\n", 1);
			g_reading = 0;
			free(g_line_buffer);
			ft_putstr_fd(PROMPT, STDOUT_FILENO);
			g_line_buffer = ft_strdup("");
			g_cursor_pos = 0;
			g_line_len = 0;
			shell->current_line = g_line_buffer;
			shell->cursor_pos = 0;
			shell->line_len = 0;
			continue ;
		}
		if (buf[0] == 10 || buf[0] == 13) /* Enter */
		{
			write(STDOUT_FILENO, "\n", 1);
			g_reading = 0;
			break ;
		}
		if (buf[0] == 127 || buf[0] == 8) /* Backspace */
		{
			delete_char(&g_line_buffer, &g_line_len, &g_cursor_pos);
			shell->current_line = g_line_buffer;
			shell->line_len = g_line_len;
			shell->cursor_pos = g_cursor_pos;
			redraw_line(&shell->termcaps, g_line_buffer, g_line_len, g_cursor_pos);
			continue ;
		}
		if (buf[0] == 27) /* Escape sequence */
		{
			ret = read(STDIN_FILENO, buf, 2);
			if (ret >= 2)
			{
				if (buf[1] == 'A') /* Up arrow */
				{
					history_up(shell);
					g_line_buffer = shell->current_line;
					g_line_len = shell->line_len;
					g_cursor_pos = shell->cursor_pos;
					redraw_line(&shell->termcaps, g_line_buffer, g_line_len, g_cursor_pos);
				}
				else if (buf[1] == 'B') /* Down arrow */
				{
					history_down(shell);
					g_line_buffer = shell->current_line;
					g_line_len = shell->line_len;
					g_cursor_pos = shell->cursor_pos;
					redraw_line(&shell->termcaps, g_line_buffer, g_line_len, g_cursor_pos);
				}
				else if (buf[1] == 'D') /* Left arrow */
				{
					if (g_cursor_pos > 0)
					{
						g_cursor_pos--;
						shell->cursor_pos = g_cursor_pos;
						if (shell->termcaps.left)
							tputs(shell->termcaps.left, 1, ft_putchar_fd);
					}
				}
				else if (buf[1] == 'C') /* Right arrow */
				{
					if (g_cursor_pos < g_line_len)
					{
						g_cursor_pos++;
						shell->cursor_pos = g_cursor_pos;
						if (shell->termcaps.right)
							tputs(shell->termcaps.right, 1, ft_putchar_fd);
					}
				}
			}
			continue ;
		}
		if (buf[0] >= 32 && buf[0] <= 126) /* Printable character */
		{
			insert_char(&g_line_buffer, &g_line_len, &g_cursor_pos, buf[0]);
			shell->current_line = g_line_buffer;
			shell->line_len = g_line_len;
			shell->cursor_pos = g_cursor_pos;
			redraw_line(&shell->termcaps, g_line_buffer, g_line_len, g_cursor_pos);
		}
	}
	signal(SIGINT, SIG_IGN);
	g_reading = 0;
	return (ft_strdup(g_line_buffer));
}
