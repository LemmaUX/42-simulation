#include "ft_select.h"

void	setup_terminal(void)
{
	int	fd;

	if (!isatty(STDIN_FILENO))
		exit(1);
	fd = open(ttyname(STDIN_FILENO), O_RDWR);
	if (fd < 0)
		fd = STDIN_FILENO;
	tcgetattr(fd, &g_select->old_term);
	g_select->new_term = g_select->old_term;
	g_select->new_term.c_lflag &= ~(ICANON | ECHO);
	g_select->new_term.c_cc[VMIN] = 1;
	g_select->new_term.c_cc[VTIME] = 0;
	tcsetattr(fd, TCSADRAIN, &g_select->new_term);
	close(fd);
	tputs(tgetstr("ti", NULL), 1, (int (*)(int))putchar);
	tputs(tgetstr("cl", NULL), 1, (int (*)(int))putchar);
}

void	restore_terminal(void)
{
	int	fd;

	fd = open(ttyname(STDIN_FILENO), O_RDWR);
	if (fd < 0)
		fd = STDIN_FILENO;
	tputs(tgetstr("te", NULL), 1, (int (*)(int))putchar);
	tcsetattr(fd, TCSADRAIN, &g_select->old_term);
	close(fd);
}
