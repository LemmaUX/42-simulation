/* ************************************************************************** */
/*   shell.c - interactive control shell (supervisorctl-like)                 */
/*   raw-mode line editor: insertion, backspace, arrows, home/end,            */
/*   ctrl-a/b/e/f/k/u/w/p/n/l, history, TAB completion of commands and        */
/*   program names.                                                           */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include "taskmaster.h"

#define PROMPT "taskmaster> "

static struct termios	g_orig;
static int		g_raw;

static const char	*g_cmds[] = {
	"status", "start", "stop", "restart", "reload", "tail",
	"help", "version", "exit", "quit", NULL
};

static void	shell_enter_raw(void)
{
	struct termios	nc;

	if (!isatty(STDIN_FILENO))
		return ;
	tcgetattr(STDIN_FILENO, &g_orig);
	nc = g_orig;
	nc.c_lflag &= ~(ICANON | ECHO);
	nc.c_iflag &= ~(IXON);
	nc.c_cc[VMIN] = 1;
	nc.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &nc);
	g_raw = 1;
}

void	shell_leave_raw(void)
{
	if (g_raw)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &g_orig);
		g_raw = 0;
	}
}

/* ---------------- line buffer ---------------- */

struct s_line {
	char	buf[TM_LINE_SIZE];
	int	len;
	int	pos;
};

static void	redraw(struct s_line *l, int clear)
{
	char	out[TM_LINE_SIZE + 64];
	int	n;
	int	i;

	n = snprintf(out, sizeof(out), "\r%s%s", PROMPT, l->buf);
	if (clear)
	{
		while (n < 78 && n < (int)sizeof(out) - 1)
			out[n++] = ' ';
		out[n] = 0;
	}
	write(STDOUT_FILENO, out, strlen(out));
	i = l->len - l->pos;
	while (i-- > 0)
		write(STDOUT_FILENO, "\b", 1);
}

static void	insert_char(struct s_line *l, char c)
{
	if (l->len >= TM_LINE_SIZE - 1)
		return ;
	memmove(l->buf + l->pos + 1, l->buf + l->pos, l->len - l->pos + 1);
	l->buf[l->pos] = c;
	l->pos++;
	l->len++;
}

static void	backspace(struct s_line *l)
{
	if (l->pos > 0)
	{
		memmove(l->buf + l->pos - 1, l->buf + l->pos,
			l->len - l->pos + 1);
		l->pos--;
		l->len--;
	}
}

static int	word_start(int p, const char *b)
{
	while (p > 0 && b[p - 1] == ' ')
		p--;
	while (p > 0 && b[p - 1] != ' ')
		p--;
	return (p);
}

/* ---------------- history ---------------- */

static char	g_hist[TM_HISTORY_MAX][TM_LINE_SIZE];
static int	g_hcount;
static int	g_hpos;

static void	hist_add(const char *s)
{
	if (!*s)
		return ;
	if (g_hcount < TM_HISTORY_MAX)
	{
		snprintf(g_hist[g_hcount], TM_LINE_SIZE, "%s", s);
		g_hcount++;
	}
	g_hpos = g_hcount;
}

static void	hist_set(struct s_line *l, const char *s)
{
	snprintf(l->buf, sizeof(l->buf), "%s", s);
	l->len = (int)strlen(l->buf);
	l->pos = l->len;
}

static void	hist_prev(struct s_line *l)
{
	if (g_hpos > 0)
	{
		g_hpos--;
		hist_set(l, g_hist[g_hpos]);
	}
}

static void	hist_next(struct s_line *l)
{
	if (g_hpos < g_hcount - 1)
	{
		g_hpos++;
		hist_set(l, g_hist[g_hpos]);
	}
	else if (g_hpos == g_hcount - 1)
	{
		g_hpos = g_hcount;
		hist_set(l, "");
	}
}

/* ---------------- completion ---------------- */

static void	complete(struct s_line *l, struct s_tm *tm)
{
	const char	**src;
	char	namebuf[256];
	const char	*cand[128];
	int		ncand;
	int		has_space;
	int		wl;
	int		i;
	int		j;
	int		common;

	has_space = (strrchr(l->buf, ' ') != NULL);
	if (has_space)
	{
		char	*last;
		int	 word_off;

		last = strrchr(l->buf, ' ');
		word_off = (int)(last - l->buf) + 1;
		wl = l->len - word_off;
		snprintf(namebuf, sizeof(namebuf), "%.*s", wl, l->buf + word_off);
		ncand = 0;
		i = 0;
		while (i < TM_MAX_PROGS && ncand < 128)
		{
			if (tm->progs[i].enabled
				&& !strncmp(tm->progs[i].name, namebuf, wl))
				cand[ncand++] = tm->progs[i].name;
			i++;
		}
	}
	else
	{
		wl = l->len;
		snprintf(namebuf, sizeof(namebuf), "%.*s", wl, l->buf);
		ncand = 0;
		src = g_cmds;
		i = 0;
		while (src[i])
		{
			if (!strncmp(src[i], namebuf, wl) && ncand < 128)
				cand[ncand++] = src[i];
			i++;
		}
	}
	if (ncand == 0)
		return ;
	write(STDOUT_FILENO, "\n", 1);
	i = 0;
	while (i < ncand)
	{
		j = 0;
		while (cand[i][j])
			j++;
		write(STDOUT_FILENO, cand[i], j);
		write(STDOUT_FILENO, "  ", 2);
		i++;
	}
	common = 256;
	i = 1;
	while (i < ncand)
	{
		j = 0;
		while (cand[0][j] && j < common && cand[0][j] == cand[i][j])
			j++;
		common = j;
		i++;
	}
	if (common > wl)
	{
		if (has_space)
		{
			char	*last;

			last = strrchr(l->buf, ' ');
			l->len = (int)(last - l->buf) + 1;
			l->buf[l->len] = 0;
		}
		else
		{
			l->len = 0;
			l->buf[0] = 0;
		}
		l->pos = l->len;
		i = wl;
		while (i < common)
		{
			insert_char(l, cand[0][i]);
			i++;
		}
		if (ncand == 1)
			insert_char(l, ' ');
		l->pos = l->len;
	}
	redraw(l, 0);
}

/* ---------------- key reading ---------------- */

static int	read_key(unsigned char *k)
{
	ssize_t	r;

	r = read(STDIN_FILENO, k, 1);
	return (r == 1);
}

/* returns 0 keep editing, 1 line done, -1 EOF */
static int	handle_key(struct s_line *l, struct s_tm *tm, unsigned char c)
{
	unsigned char	seq[2];

	if (c == '\n' || c == '\r')
	{
		write(STDOUT_FILENO, "\n", 1);
		return (1);
	}
	if (c == 3)
	{
		write(STDOUT_FILENO, "^C\n", 3);
		hist_set(l, "");
		redraw(l, 1);
		return (0);
	}
	if (c == 4)
	{
		if (l->len == 0)
			return (-1);
		return (0);
	}
	if (c == 9)
	{
		complete(l, tm);
		return (0);
	}
	if (c == 27)
	{
		if (read_key(seq) && seq[0] == '[' && read_key(seq + 1))
		{
			if (seq[1] == 'A')
				hist_prev(l);
			else if (seq[1] == 'B')
				hist_next(l);
			else if (seq[1] == 'C' && l->pos < l->len)
				l->pos++;
			else if (seq[1] == 'D' && l->pos > 0)
				l->pos--;
			else if (seq[1] == 'H')
				l->pos = 0;
			else if (seq[1] == 'F')
				l->pos = l->len;
			else if (seq[1] == '3')
			{
				unsigned char	tilde;

				if (read_key(&tilde) && tilde == '~'
					&& l->pos < l->len)
				{
					memmove(l->buf + l->pos,
						l->buf + l->pos + 1,
						l->len - l->pos);
					l->len--;
				}
			}
		}
		redraw(l, 0);
		return (0);
	}
	if (c == 1)
		l->pos = 0;
	else if (c == 5)
		l->pos = l->len;
	else if (c == 2)
	{
		if (l->pos > 0)
			l->pos--;
	}
	else if (c == 6)
	{
		if (l->pos < l->len)
			l->pos++;
	}
	else if (c == 8 || c == 127)
		backspace(l);
	else if (c == 11)
	{
		l->buf[l->pos] = 0;
		l->len = l->pos;
	}
	else if (c == 21)
		hist_set(l, "");
	else if (c == 23)
	{
		int	ws;

		ws = word_start(l->pos, l->buf);
		memmove(l->buf + ws, l->buf + l->pos, l->len - l->pos + 1);
		l->len -= l->pos - ws;
		l->pos = ws;
	}
	else if (c == 16)
		hist_prev(l);
	else if (c == 14)
		hist_next(l);
	else if (c == 12)
	{
		write(STDOUT_FILENO, "\033[2J\033[H", 6);
		redraw(l, 0);
		return (0);
	}
	else if (c >= 32 && c < 127)
		insert_char(l, (char)c);
	redraw(l, 0);
	return (0);
}

/* ---------------- commands ---------------- */

static void	print_status(struct s_tm *tm)
{
	int	i;
	int	j;
	int	shown;

	printf("%-20s %-16s %8s %s\n", "NAME", "STATE", "PID", "CMD");
	i = 0;
	while (i < TM_MAX_PROGS)
	{
		if (tm->progs[i].enabled)
		{
			shown = 0;
			j = 0;
			while (j < TM_MAX_PROCS)
			{
				if (tm->procs[j].used
					&& tm->procs[j].prog_idx == i)
				{
					printf("%-20s %-16s %8d %s\n",
						tm->progs[i].name,
						tm_state_name(tm->procs[j].state),
						(int)tm->procs[j].pid,
						tm->progs[i].cmd);
					shown = 1;
				}
				j++;
			}
			if (!shown)
				printf("%-20s %-16s %8s %s\n", tm->progs[i].name,
					"STOPPED", "-", tm->progs[i].cmd);
		}
		i++;
	}
}

static void	do_tail(const char *path)
{
	char	*ring[10];
	char	line[1024];
	FILE	*f;
	int	n;
	int	i;

	i = 0;
	while (i < 10)
		ring[i++] = NULL;
	f = fopen(path, "r");
	if (!f)
	{
		printf("tail: cannot open %s\n", path);
		return ;
	}
	n = 0;
	while (fgets(line, sizeof(line), f))
	{
		if (n >= 10)
			free(ring[0]);
		else
			n++;
		memmove(&ring[0], &ring[1], sizeof(char *) * 9);
		ring[9] = strdup(line);
	}
	fclose(f);
	i = 0;
	while (i < n)
	{
		fputs(ring[i], stdout);
		free(ring[i]);
		i++;
	}
}

static void	prog_action(struct s_tm *tm, char **argv, int ac, int what)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (!strcmp(argv[i], "all"))
		{
			int	j;

			j = 0;
			while (j < TM_MAX_PROGS)
			{
				if (tm->progs[j].enabled)
				{
					if (what == 0)
						tm_start(tm, tm->progs[j].name);
					else if (what == 1)
						tm_stop(tm, tm->progs[j].name);
					else
						tm_restart(tm, tm->progs[j].name);
				}
				j++;
			}
		}
		else if (!tm_find_prog(tm, argv[i]))
			printf("no such program: %s\n", argv[i]);
		else
		{
			if (what == 0)
				tm_start(tm, argv[i]);
			else if (what == 1)
				tm_stop(tm, argv[i]);
			else
				tm_restart(tm, argv[i]);
		}
		i++;
	}
}

void	tm_cmd(struct s_tm *tm, char *line)
{
	char	*argv[8];
	int	ac;

	while (*line == ' ')
		line++;
	if (!*line)
		return ;
	ac = 0;
	argv[ac++] = line;
	while (*line && ac < 7)
	{
		if (*line == ' ' || *line == '\t')
		{
			*line++ = 0;
			while (*line == ' ' || *line == '\t')
				line++;
			if (*line)
				argv[ac++] = line;
		}
		else
			line++;
	}
	argv[ac] = NULL;
	if (!strcmp(argv[0], "status"))
		print_status(tm);
	else if (!strcmp(argv[0], "start") && ac > 1)
		prog_action(tm, argv, ac, 0);
	else if (!strcmp(argv[0], "stop") && ac > 1)
		prog_action(tm, argv, ac, 1);
	else if (!strcmp(argv[0], "restart") && ac > 1)
		prog_action(tm, argv, ac, 2);
	else if (!strcmp(argv[0], "reload"))
		tm_reload(tm);
	else if (!strcmp(argv[0], "tail"))
	{
		if (ac > 1)
			do_tail(argv[1]);
		else
			printf("usage: tail <file>\n");
	}
	else if (!strcmp(argv[0], "help"))
	{
		printf("commands:\n");
		printf("  status            show all supervised programs\n");
		printf("  start <prog>...   start program(s) or 'all'\n");
		printf("  stop <prog>...    stop program(s) or 'all'\n");
		printf("  restart <prog>    restart a program\n");
		printf("  reload            reload the configuration file\n");
		printf("  tail <file>       dump tail of a log file\n");
		printf("  version           show version\n");
		printf("  exit / quit       stop taskmaster and leave\n");
	}
	else if (!strcmp(argv[0], "version"))
		printf("taskmaster 1.0 (42 - job control daemon)\n");
	else if (!strcmp(argv[0], "exit") || !strcmp(argv[0], "quit"))
		tm->running = 0;
	else
		printf("unknown command: %s (try 'help')\n", argv[0]);
}

/*
** One iteration of the shell, called from the main loop when stdin is ready.
*/
int	tm_shell_step(struct s_tm *tm)
{
	static struct s_line	l;
	unsigned char	c;

	if (!g_raw && isatty(STDIN_FILENO))
	{
		shell_enter_raw();
		write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
	}
	while (read_key(&c))
	{
		int	r;

		r = handle_key(&l, tm, c);
		if (r == 1)
		{
			char	linecopy[TM_LINE_SIZE];

			snprintf(linecopy, sizeof(linecopy), "%s", l.buf);
			hist_set(&l, "");
			hist_add(linecopy);
			tm_cmd(tm, linecopy);
			if (!tm->running)
				return (1);
			if (g_raw)
				write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
			return (0);
		}
		if (r == -1)
		{
			tm->running = 0;
			return (1);
		}
	}
	return (0);
}
