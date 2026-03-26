#include "ft_select.h"
#include <signal.h>

void	setup_signals(void)
{
	signal(SIGWINCH, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGTSTP, signal_handler);
	signal(SIGCONT, signal_handler);
}

void	signal_handler(int sig)
{
	if (sig == SIGWINCH)
		handle_key(g_select, KEY_SIGWINCH);
	else if (sig == SIGTSTP)
	{
		restore_terminal();
		signal(SIGTSTP, SIG_DFL);
		kill(getpid(), SIGTSTP);
	}
	else if (sig == SIGCONT)
	{
		setup_terminal();
		handle_key(g_select, KEY_SIGWINCH);
	}
	else
	{
		restore_terminal();
		exit(0);
	}
}
