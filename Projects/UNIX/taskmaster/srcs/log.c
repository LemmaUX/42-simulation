/* ************************************************************************** */
/*   log.c - event logging to a local file                                    */
/* ************************************************************************** */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "taskmaster.h"

void	tm_log_open(struct s_tm *tm, const char *path)
{
	if (tm->log)
		fclose(tm->log);
	if (!path || !*path)
		path = "taskmaster.log";
	tm->log = fopen(path, "a");
	if (*path && path != tm->logpath)
		snprintf(tm->logpath, 512, "%s", path);
}

void	tm_log(struct s_tm *tm, const char *fmt, ...)
{
	char	buf[TM_LINE_SIZE];
	va_list	ap;
	char	ts[32];
	time_t	t;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	t = time(NULL);
	snprintf(ts, sizeof(ts), "%s", ctime(&t));
	if (strlen(ts) > 0)
		ts[strlen(ts) - 1] = 0;
	if (tm->log)
	{
		fprintf(tm->log, "%s %s\n", ts, buf);
		fflush(tm->log);
	}
}

void	tm_log_close(struct s_tm *tm)
{
	if (tm->log)
	{
		fclose(tm->log);
		tm->log = NULL;
	}
}
