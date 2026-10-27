/* ************************************************************************** */
/*   config.c - INI-style configuration parser (supervisor-like)              */
/*   sections: [program:name] with keys cmd, numprocs, autostart, autorestart,*/
/*   exitcodes, startsecs, startretries, stopsignal, stopwaitsecs,            */
/*   stdout_logfile, stderr_logfile, environment, directory, umask            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "taskmaster.h"

static char	*trim(char *s)
{
	char	*e;

	while (*s == ' ' || *s == '\t')
		s++;
	e = s + strlen(s);
	while (e > s && (e[-1] == ' ' || e[-1] == '\t' || e[-1] == '\n'
			|| e[-1] == '\r'))
		e--;
	*e = 0;
	return (s);
}

static int	sig_from_name(const char *name)
{
	static const struct { const char *n; int s; } tabs[] = {
		{"TERM", SIGTERM}, {"INT", SIGINT}, {"QUIT", SIGQUIT},
		{"HUP", SIGHUP}, {"USR1", SIGUSR1}, {"USR2", SIGUSR2},
		{"KILL", SIGKILL}, {"STOP", SIGSTOP}, {"CONT", SIGCONT},
		{"PIPE", SIGPIPE}, {"ALRM", SIGALRM}, {"ABRT", SIGABRT},
	};
	size_t	i;

	i = 0;
	while (i < sizeof(tabs) / sizeof(tabs[0]))
	{
		if (!strcasecmp(tabs[i].n, name))
			return (tabs[i].s);
		i++;
	}
	if (*name >= '0' && *name <= '9')
		return (atoi(name));
	return (-1);
}

static void	parse_env(struct s_prog *p, char *v)
{
	char	buf[1024];
	char	*tok;
	char	*eq;
	char	*key;
	char	*val;
	size_t	vl;

	snprintf(buf, sizeof(buf), "%s", v);
	tok = strtok(buf, ",");
	while (tok && p->nenv < 16)
	{
		eq = strchr(tok, '=');
		if (eq)
		{
			*eq = 0;
			key = trim(tok);
			val = trim(eq + 1);
			vl = strlen(val);
			if (vl >= 2 && ((val[0] == '"' && val[vl - 1] == '"')
					|| (val[0] == '\'' && val[vl - 1] == '\'')))
			{
				val[vl - 1] = 0;
				val++;
			}
			snprintf(p->env[p->nenv++], 256, "%s=%s", key, val);
		}
		tok = strtok(NULL, ",");
	}
}

static void	set_key(struct s_prog *p, const char *k, char *v)
{
	int	i;

	if (!strcmp(k, "cmd"))
		snprintf(p->cmd, sizeof(p->cmd), "%s", v);
	else if (!strcmp(k, "numprocs"))
		p->numprocs = atoi(v);
	else if (!strcmp(k, "autostart"))
		p->autostart = (!strcasecmp(v, "true") || !strcmp(v, "1"));
	else if (!strcmp(k, "autorestart"))
	{
		if (!strcasecmp(v, "true"))
			p->autorestart = RST_ALWAYS;
		else if (!strcasecmp(v, "false"))
			p->autorestart = RST_NEVER;
		else
			p->autorestart = RST_UNEXPECTED;
	}
	else if (!strcmp(k, "exitcodes") || !strcmp(k, "expected_exitcodes"))
	{
		p->nexitcodes = 0;
		{
			char	*tok;

			tok = strtok(v, ", ");
			while (tok && p->nexitcodes < 32)
			{
				p->exitcodes[p->nexitcodes++] = atoi(tok);
				tok = strtok(NULL, ", ");
			}
		}
	}
	else if (!strcmp(k, "startsecs") || !strcmp(k, "starttime"))
		p->starttime = atoi(v);
	else if (!strcmp(k, "startretries"))
		p->startretries = atoi(v);
	else if (!strcmp(k, "stopsignal"))
	{
		i = sig_from_name(v);
		if (i > 0)
			p->stopsignal = i;
	}
	else if (!strcmp(k, "stopwaitsecs") || !strcmp(k, "stoptime"))
		p->stoptime = atoi(v);
	else if (!strcmp(k, "stdout_logfile") || !strcmp(k, "stdout"))
	{
		if (!strcasecmp(v, "NONE") || !strcmp(v, "-"))
			p->discard_out = 1;
		snprintf(p->stdout_path, sizeof(p->stdout_path), "%s", v);
	}
	else if (!strcmp(k, "stderr_logfile") || !strcmp(k, "stderr"))
	{
		if (!strcasecmp(v, "NONE") || !strcmp(v, "-"))
			p->discard_err = 1;
		snprintf(p->stderr_path, sizeof(p->stderr_path), "%s", v);
	}
	else if (!strcmp(k, "directory") || !strcmp(k, "workingdir"))
		snprintf(p->workingdir, sizeof(p->workingdir), "%s", v);
	else if (!strcmp(k, "umask"))
	{
		p->umask = (int)strtol(v, NULL, 8);
		p->has_umask = 1;
	}
	else if (!strcmp(k, "environment"))
		parse_env(p, v);
}

static struct s_prog	*new_prog(struct s_tm *tm, const char *name)
{
	int	i;

	i = 0;
	while (i < TM_MAX_PROGS)
	{
		if (!tm->progs[i].enabled)
		{
			memset(&tm->progs[i], 0, sizeof(struct s_prog));
			tm->progs[i].enabled = 1;
			snprintf(tm->progs[i].name, 256, "%s", name);
			tm->progs[i].numprocs = 1;
			tm->progs[i].autorestart = RST_UNEXPECTED;
			tm->progs[i].starttime = 1;
			tm->progs[i].startretries = 3;
			tm->progs[i].stopsignal = SIGTERM;
			tm->progs[i].stoptime = 10;
			tm->progs[i].exitcodes[0] = 0;
			tm->progs[i].nexitcodes = 1;
			return (&tm->progs[i]);
		}
		i++;
	}
	return (NULL);
}

int	tm_config_load(const char *path, struct s_tm *tm)
{
	FILE	*f;
	char	line[TM_LINE_SIZE];
	struct s_prog	*cur;
	int	globals;
	char	*s;
	char	*eq;
	char	*k;
	char	*v;
	char	*end;

	f = fopen(path, "r");
	if (!f)
		return (-1);
	cur = NULL;
	globals = 1;
	while (fgets(line, sizeof(line), f))
	{
		s = trim(line);
		if (!*s || *s == ';' || *s == '#')
			continue ;
		if (*s == '[')
		{
			end = strchr(s, ']');
			if (!end)
				continue ;
			*end = 0;
			s++;
			globals = 0;
			cur = NULL;
			if (!strncasecmp(s, "program:", 8))
				cur = new_prog(tm, trim(s + 8));
			continue ;
		}
		eq = strchr(s, '=');
		if (!eq)
			continue ;
		*eq = 0;
		k = trim(s);
		v = trim(eq + 1);
		if (globals)
		{
			if (!strcmp(k, "logfile"))
				snprintf(tm->logpath, 512, "%s", v);
			continue ;
		}
		if (cur)
			set_key(cur, k, v);
	}
	fclose(f);
	return (0);
}

struct s_prog	*tm_find_prog(struct s_tm *tm, const char *name)
{
	int	i;

	i = 0;
	while (i < TM_MAX_PROGS)
	{
		if (tm->progs[i].enabled && !strcmp(tm->progs[i].name, name))
			return (&tm->progs[i]);
		i++;
	}
	return (NULL);
}
