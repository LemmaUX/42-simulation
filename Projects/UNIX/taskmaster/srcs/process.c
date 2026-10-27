/* ************************************************************************** */
/*   process.c - spawn / supervise / restart / stop child processes           */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include "taskmaster.h"

const char	*tm_state_name(enum e_state s)
{
	if (s == ST_STOPPED)
		return ("STOPPED");
	if (s == ST_STARTING)
		return ("STARTING");
	if (s == ST_RUNNING)
		return ("RUNNING");
	if (s == ST_BACKOFF)
		return ("BACKOFF");
	if (s == ST_STOPPING)
		return ("STOPPING");
	if (s == ST_EXITED)
		return ("EXITED");
	if (s == ST_FATAL)
		return ("FATAL");
	return ("UNKNOWN");
}

static int	exitcode_expected(struct s_prog *p, int code)
{
	int	i;

	i = 0;
	while (i < p->nexitcodes)
	{
		if (p->exitcodes[i] == code)
			return (1);
		i++;
	}
	return (0);
}

static struct s_proc	*find_free_proc(struct s_tm *tm)
{
	int	i;

	i = 0;
	while (i < TM_MAX_PROCS)
	{
		if (!tm->procs[i].used)
			return (&tm->procs[i]);
		i++;
	}
	return (NULL);
}

static int	count_procs(struct s_tm *tm, int prog_idx)
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

/* split cmd into argv: supports spaces and "double quotes" */
static int	split_cmd(char *cmd, char **argv, int max)
{
	int	n;
	char	*s;
	char	*d;

	n = 0;
	s = cmd;
	while (*s && n < max - 1)
	{
		while (*s == ' ' || *s == '\t')
			s++;
		if (!*s)
			break ;
		d = s;
		argv[n++] = d;
		while (*s && *s != ' ' && *s != '\t')
		{
			if (*s == '"')
			{
				s++;
				while (*s && *s != '"')
					*d++ = *s++;
				if (*s == '"')
					s++;
			}
			else
				*d++ = *s++;
		}
		*d = 0;
	}
	argv[n] = NULL;
	return (n);
}

void	tm_spawn(struct s_tm *tm, struct s_prog *p, int n)
{
	struct s_proc	*pr;
	pid_t	pid;
	char	*argv[64];
	char	cmdcopy[1024];
	int	outfd;
	int	errfd;
	int	devnull;
	int	i;

	pr = find_free_proc(tm);
	if (!pr)
		return ;
	outfd = -1;
	errfd = -1;
	if (p->stdout_path[0] && !p->discard_out)
		outfd = open(p->stdout_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (p->stderr_path[0] && !p->discard_err)
		errfd = open(p->stderr_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	snprintf(cmdcopy, sizeof(cmdcopy), "%s", p->cmd);
	split_cmd(cmdcopy, argv, 64);
	pid = fork();
	if (pid < 0)
	{
		tm_log(tm, "spawn %s-%d: fork failed: %s", p->name, n,
			strerror(errno));
		return ;
	}
	if (pid == 0)
	{
		signal(SIGHUP, SIG_DFL);
		signal(SIGCHLD, SIG_DFL);
		if (outfd >= 0)
			dup2(outfd, STDOUT_FILENO);
		else if (p->discard_out)
		{
			devnull = open("/dev/null", O_WRONLY);
			if (devnull >= 0)
				dup2(devnull, STDOUT_FILENO);
		}
		if (errfd >= 0)
			dup2(errfd, STDERR_FILENO);
		else if (p->discard_err)
		{
			devnull = open("/dev/null", O_WRONLY);
			if (devnull >= 0)
				dup2(devnull, STDERR_FILENO);
		}
		if (p->has_umask)
			umask(p->umask);
		if (p->workingdir[0])
		{
			if (chdir(p->workingdir) < 0)
				_exit(127);
		}
		i = 0;
		while (i < p->nenv)
		{
			putenv(strdup(p->env[i]));
			i++;
		}
		execvp(argv[0], argv);
		_exit(127);
	}
	if (outfd >= 0)
		close(outfd);
	if (errfd >= 0)
		close(errfd);
	pr->used = 1;
	pr->pid = pid;
	pr->prog_idx = (int)(p - tm->progs);
	pr->state = ST_STARTING;
	pr->started_at = time(NULL);
	pr->state_at = pr->started_at;
	pr->retries = 0;
	pr->expected_stop = 0;
	pr->status = 0;
	tm_log(tm, "spawned %s-%d (pid %d) cmd '%s'", p->name, n, (int)pid,
		p->cmd);
}

void	tm_start(struct s_tm *tm, const char *name)
{
	struct s_prog	*p;
	int	have;
	int	i;

	p = tm_find_prog(tm, name);
	if (!p)
		return ;
	have = count_procs(tm, (int)(p - tm->progs));
	i = have;
	while (i < p->numprocs)
	{
		tm_spawn(tm, p, i);
		i++;
	}
}

void	tm_stop(struct s_tm *tm, const char *name)
{
	struct s_prog	*p;
	int	i;

	p = tm_find_prog(tm, name);
	if (!p)
		return ;
	i = 0;
	while (i < TM_MAX_PROCS)
	{
		if (tm->procs[i].used
			&& tm->procs[i].prog_idx == (int)(p - tm->progs))
		{
			tm->procs[i].expected_stop = 1;
			if (tm->procs[i].state == ST_STOPPING)
				kill(tm->procs[i].pid, SIGKILL);
			else
			{
				kill(tm->procs[i].pid, p->stopsignal);
				tm->procs[i].state = ST_STOPPING;
				tm->procs[i].state_at = time(NULL);
				tm_log(tm, "sent signal %d to %s (slot %d) pid %d",
					p->stopsignal, p->name, i,
					(int)tm->procs[i].pid);
			}
		}
		i++;
	}
}

void	tm_restart(struct s_tm *tm, const char *name)
{
	struct s_prog	*p;
	int	i;

	p = tm_find_prog(tm, name);
	if (!p)
		return ;
	tm_stop(tm, name);
	i = 0;
	while (i < 100 && count_procs(tm, (int)(p - tm->progs)) > 0)
	{
		usleep(100000);
		tm_check_processes(tm);
		i++;
	}
	tm_start(tm, name);
	tm_log(tm, "restart: %s", name);
}

static void	set_state(struct s_proc *pr, enum e_state s)
{
	pr->state = s;
	pr->state_at = time(NULL);
}

/* reap: collect one dead child, decide whether to respawn it */
static void	reap(struct s_tm *tm, pid_t pid, int status)
{
	int		i;
	struct s_prog	*p;
	int		code;
	int		unexpected;
	int		want_alive;
	int		idx;
	int		retries;

	i = 0;
	while (i < TM_MAX_PROCS)
	{
		if (tm->procs[i].used && tm->procs[i].pid == pid)
		{
			p = &tm->progs[tm->procs[i].prog_idx];
			code = -1;
			if (WIFEXITED(status))
				code = WEXITSTATUS(status);
			tm_log(tm, "%s (pid %d) exited code=%d signal=%d",
				p->name, (int)pid, code,
				WIFSIGNALED(status) ? WTERMSIG(status) : 0);
			unexpected = !(code >= 0 && exitcode_expected(p, code));
			if (!unexpected || tm->procs[i].expected_stop)
			{
				tm->procs[i].used = 0;
				return ;
			}
			want_alive = (p->autorestart == RST_ALWAYS
					|| p->autorestart == RST_UNEXPECTED);
			tm->procs[i].retries++;
			if (!want_alive)
			{
				tm->procs[i].used = 0;
				return ;
			}
			if (tm->procs[i].retries > p->startretries)
			{
				set_state(&tm->procs[i], ST_FATAL);
				tm_log(tm, "%s FATAL: too many restart failures",
					p->name);
				return ;
			}
			idx = tm->procs[i].prog_idx;
			retries = tm->procs[i].retries;
			tm->procs[i].used = 0;
			tm_start(tm, tm->progs[idx].name);
			i = 0;
			while (i < TM_MAX_PROCS)
			{
				if (tm->procs[i].used
					&& tm->procs[i].prog_idx == idx
					&& tm->procs[i].state == ST_STARTING)
				{
					tm->procs[i].retries = retries;
					break ;
				}
				i++;
			}
			return ;
		}
		i++;
	}
}

void	tm_check_processes(struct s_tm *tm)
{
	pid_t	pid;
	int	status;
	int	i;
	time_t	now;
	struct s_prog	*p;

	tm->got_sigchld = 0;
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
		reap(tm, pid, status);
	now = time(NULL);
	i = 0;
	while (i < TM_MAX_PROCS)
	{
		if (tm->procs[i].used)
		{
			p = &tm->progs[tm->procs[i].prog_idx];
			if (tm->procs[i].state == ST_STARTING)
			{
				if (now - tm->procs[i].started_at >= p->starttime)
				{
					tm->procs[i].state = ST_RUNNING;
					tm->procs[i].retries = 0;
					tm_log(tm, "%s (pid %d) started", p->name,
						(int)tm->procs[i].pid);
				}
			}
			else if (tm->procs[i].state == ST_STOPPING)
			{
				if (now - tm->procs[i].state_at >= p->stoptime)
				{
					kill(tm->procs[i].pid, SIGKILL);
					tm_log(tm, "killing %s (pid %d) after stopwait",
						p->name, (int)tm->procs[i].pid);
					tm->procs[i].state_at = now;
				}
			}
		}
		i++;
	}
}

void	tm_stop_all(struct s_tm *tm)
{
	int	i;
	int	j;

	i = 0;
	while (i < TM_MAX_PROGS)
	{
		if (tm->progs[i].enabled)
			tm_stop(tm, tm->progs[i].name);
		i++;
	}
	j = 0;
	while (j < 50)
	{
		usleep(100000);
		tm_check_processes(tm);
		j++;
	}
}
