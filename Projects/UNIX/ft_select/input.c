#include "ft_select.h"

int	read_key(void)
{
	char	buffer[20];
	int		bytes;

	ft_memset(buffer, 0, 20);
	bytes = read(STDIN_FILENO, buffer, 19);
	if (bytes <= 0)
		return (-1);
	if (buffer[0] == 27)
	{
		if (bytes == 1)
			return (KEY_ESCAPE);
		if (buffer[1] == '[')
		{
			if (buffer[2] == 'A')
				return (KEY_UP);
			if (buffer[2] == 'B')
				return (KEY_DOWN);
			if (buffer[2] == 'C')
				return (KEY_RIGHT);
			if (buffer[2] == 'D')
				return (KEY_LEFT);
			if (buffer[2] == '3' && buffer[3] == '~')
				return (KEY_DELETE);
		}
	}
	if (buffer[0] == ' ')
		return (KEY_SPACE);
	if (buffer[0] == '\n' || buffer[0] == '\r')
		return (KEY_RETURN);
	if (buffer[0] == 127 || buffer[0] == '\b')
		return (KEY_BACKSPACE);
	return (buffer[0]);
}

void	move_cursor(int col, int row)
{
	char	*cmd;

	cmd = tgoto(tgetstr("cm", NULL), col, row);
	tputs(cmd, 1, (int (*)(int))putchar);
}

void	clear_display(t_select *select)
{
	tputs(tgetstr("cl", NULL), 1, (int (*)(int))putchar);
	fflush(stdout);
}
