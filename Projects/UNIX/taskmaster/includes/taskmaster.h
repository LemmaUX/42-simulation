/* ************************************************************************** */
/*                                                                            */
/*   taskmaster.h - job control daemon                                        */
/*                                                                            */
/*   authors: zaz@42.fr / martine@42.fr                                       */
/* ************************************************************************** */

#ifndef TASKMASTER_H
# define TASKMASTER_H

#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>

#define TM_MAX_PROGS      64
#define TM_MAX_PROCS      512
#define TM_LINE_SIZE      4096
#define TM_HISTORY_MAX    100

/* autorestart policy */
enum e_autorestart {
	RST_NEVER = 0,
	RST_ALWAYS,
	RST_UNEXPECTED
};

/* process state machine (like supervisor) */
enum e_state {
	ST_STOPPED = 0,
	ST_STARTING,
	ST_RUNNING,
	ST_BACKOFF,
	ST_STOPPING,
	ST_EXITED,
	ST_FATAL,
	ST_UNKNOWN
};

/* one supervised program (config section) */
struct s_prog {
	char	name[256];
	char	cmd[1024];
	int	numprocs;
	int	autostart;
	enum e_autorestart	autorestart;
	int	exitcodes[32];
	int	nexitcodes;
	int	starttime;
	int	startretries;
	int	stopsignal;
	int	stoptime;
	char	stdout_path[512];
	char	stderr_path[512];
	int	discard_out;
	int	discard_err;
	char	workingdir[512];
	int	umask;
	int	has_umask;
	char	env[16][256];
	int	nenv;
	int	enabled;
};

/* one live child process */
struct s_proc {
	int	used;
	pid_t	pid;
	int	prog_idx;
	enum e_state	state;
	time_t	started_at;
	time_t	state_at;
	int	retries;
	int	expected_stop;
	int	status;
};

/* global runtime context */
struct s_tm {
	struct s_prog	progs[TM_MAX_PROGS];
	struct s_proc	procs[TM_MAX_PROCS];
	FILE	*log;
	char	logpath[512];
	volatile int	got_sighup;
	volatile int	got_sigchld;
	int	running;
};

/* config.c */
int	tm_config_load(const char *path, struct s_tm *tm);
struct s_prog	*tm_find_prog(struct s_tm *tm, const char *name);

/* log.c */
void	tm_log_open(struct s_tm *tm, const char *path);
void	tm_log(struct s_tm *tm, const char *fmt, ...);
void	tm_log_close(struct s_tm *tm);

/* process.c */
void	tm_spawn(struct s_tm *tm, struct s_prog *p, int n);
void	tm_start(struct s_tm *tm, const char *name);
void	tm_stop(struct s_tm *tm, const char *name);
void	tm_restart(struct s_tm *tm, const char *name);
void	tm_check_processes(struct s_tm *tm);
void	tm_stop_all(struct s_tm *tm);
const char	*tm_state_name(enum e_state s);

/* shell.c */
void	tm_cmd(struct s_tm *tm, char *line);
int	tm_shell_step(struct s_tm *tm);
void	shell_leave_raw(void);

/* main.c */
void	tm_reload(struct s_tm *tm);

#endif
