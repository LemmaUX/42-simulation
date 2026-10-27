/* ************************************************************************** */
/*   main.c - taskmaster: job control daemon (42 project)                     */
/*                                                                            */
/*   usage: ./taskmaster [-c configfile]                                      */
/*   stays in foreground, provides a control shell, supervises children,      */
/*   reloads configuration on SIGHUP without touching unchanged processes.    */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/select.h>
#include <getopt.h>
#include "taskmaster.h"

static struct s_tm	g_tm;
static char		g_config_path[512];

static void	on_sighup(int sig)
{
	(void)sig;
	g_tm.got_sighup = 1;
}

static void	on_sigchld(int sig)
{
	(void)sig;
	g_tm.got_sigchld = 1;
}

static void	on_sigint(int sig)
{
	(void)sig;
	g_tm.running = 0;
}

static void	setup_signals(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = on_sighup;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGHUP, &sa, NULL);
	sa.sa_handler = on_sigchld;
	sigaction(SIGCHLD, &sa, NULL);
	sa.sa_handler = on_sigint;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGPIPE, SIG_IGN);
}

/*
** Reload: parse a fresh config into a temp context, then merge it into the
** running one:
**  - programs whose definition is identical are left untouched (their child
**    processes keep running),
**  - changed programs get stopped and restarted with the new definition,
**  - removed programs get their processes stopped,
**  - new programs are added (and started if autostart).
*/
/* count live (used) slots of a program */
static int	count_alive(struct s_tm *tm, int prog_idx)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (i < TM_MAX_PROCS)
	{
		if (tm->procs[i].used && tm->procs[i].prog_idx == prog_idx)
			n++;
		i++;
	}
	return (n);
}

static void	reload_config(void)
{
	static struct s_tm	fresh;
	struct s_prog	*np;
	int	i;
	int	j;

	memset(&fresh, 0, sizeof(fresh));
	if (tm_config_load(g_config_path, &fresh) < 0)
	{
		tm_log(&g_tm, "reload: cannot read %s, keeping current config",
			g_config_path);
		return ;
	}
	i = 0;
	while (i < TM_MAX_PROGS)
	{
		if (g_tm.progs[i].enabled)
		{
			np = tm_find_prog(&fresh, g_tm.progs[i].name);
			if (!np)
			{
				tm_log(&g_tm, "reload: program '%s' removed -> stopping",
					g_tm.progs[i].name);
				tm_stop(&g_tm, g_tm.progs[i].name);
				g_tm.progs[i].enabled = 0;
			}
			else if (memcmp(np, &g_tm.progs[i],
					sizeof(struct s_prog)) != 0)
			{
				tm_log(&g_tm, "reload: program '%s' changed -> restarting",
					g_tm.progs[i].name);
				tm_stop(&g_tm, g_tm.progs[i].name);
				j = 0;
				while (j < 100)
				{
					usleep(100000);
					tm_check_processes(&g_tm);
					if (count_alive(&g_tm, i) == 0)
						break ;
					j++;
				}
				memcpy(&g_tm.progs[i], np, sizeof(struct s_prog));
				if (g_tm.progs[i].autostart)
					tm_start(&g_tm, g_tm.progs[i].name);
			}
		}
		i++;
	}
	i = 0;
	while (i < TM_MAX_PROGS)
	{
		if (fresh.progs[i].enabled
			&& !tm_find_prog(&g_tm, fresh.progs[i].name))
		{
			tm_log(&g_tm, "reload: program '%s' added",
				fresh.progs[i].name);
			j = 0;
			while (j < TM_MAX_PROGS)
			{
				if (!g_tm.progs[j].enabled)
				{
					memcpy(&g_tm.progs[j], &fresh.progs[i],
						sizeof(struct s_prog));
					break ;
				}
				j++;
			}
			if (j < TM_MAX_PROGS && g_tm.progs[j].autostart)
				tm_start(&g_tm, g_tm.progs[j].name);
		}
		i++;
	}
	tm_log(&g_tm, "configuration reloaded from %s", g_config_path);
}

void	tm_reload(struct s_tm *tm)
{
	(void)tm;
	reload_config();
}

static void	usage(const char *av0)
{
	printf("usage: %s [-c configfile]\n", av0);
	printf("  -c FILE   configuration file (default: taskmaster.conf)\n");
}

int	main(int ac, char **av)
{
	int	c;
	int	i;

	g_config_path[0] = 0;
	while ((c = getopt(ac, av, "c:h")) != -1)
	{
		if (c == 'c')
			snprintf(g_config_path, 512, "%s", optarg);
		else
		{
			usage(av[0]);
			return (0);
		}
	}
	if (!*g_config_path)
		snprintf(g_config_path, 512, "taskmaster.conf");
	setup_signals();
	g_tm.running = 1;
	if (tm_config_load(g_config_path, &g_tm) < 0)
	{
		fprintf(stderr, "taskmaster: cannot load config '%s'\n",
			g_config_path);
		return (1);
	}
	if (!*g_tm.logpath)
		snprintf(g_tm.logpath, 512, "taskmaster.log");
	tm_log_open(&g_tm, g_tm.logpath);
	tm_log(&g_tm, "taskmaster started (pid %d) config=%s",
		(int)getpid(), g_config_path);
	i = 0;
	while (i < TM_MAX_PROGS)
	{
		if (g_tm.progs[i].enabled && g_tm.progs[i].autostart)
			tm_start(&g_tm, g_tm.progs[i].name);
		i++;
	}
	printf("taskmaster control shell - type 'help' for commands.\n");
	/* main loop: supervise children + service the interactive shell.
	** select() with a 1s timeout polls process states every second while
	** staying responsive to input and signals. */
	while (g_tm.running)
	{
		if (g_tm.got_sighup)
		{
			g_tm.got_sighup = 0;
			reload_config();
		}
		tm_check_processes(&g_tm);
		{
			fd_set	rfds;
			struct timeval	tv;
			int	n;

			FD_ZERO(&rfds);
			FD_SET(STDIN_FILENO, &rfds);
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			n = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
			if (n > 0 && FD_ISSET(STDIN_FILENO, &rfds))
				tm_shell_step(&g_tm);
		}
	}
	shell_leave_raw();
	tm_log(&g_tm, "taskmaster shutting down: stopping all programs");
	tm_stop_all(&g_tm);
	tm_log(&g_tm, "taskmaster stopped");
	tm_log_close(&g_tm);
	return (0);
}
