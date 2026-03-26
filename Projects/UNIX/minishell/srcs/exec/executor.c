/* ************************************************************************** */
/*                                                                            */
/*   executor.c - Ejecucion de comandos                                       */
/*                                                                            */
/*   El executor recibe la lista enlazada de t_cmd y ejecuta:                 */
/*     - Comando simple (sin pipe): fork + execve (o builtin directo)         */
/*     - Pipeline (con pipes): fork por cada comando, conectados con pipe()   */
/*                                                                            */
/*   Flujo para un comando simple:                                            */
/*     1. Es builtin? -> ejecutar en el proceso padre                         */
/*     2. No es builtin? -> fork + buscar en PATH + execve                    */
/*                                                                            */
/*   Flujo para pipeline (cmd1 | cmd2 | cmd3):                                */
/*     1. Crear N-1 pipes                                                     */
/*     2. Fork para cada comando                                              */
/*     3. Cada hijo configura sus fd (dup2) y ejecuta                         */
/*     4. El padre cierra todos los pipes y espera a todos los hijos          */
/*     5. El exit status es el del ULTIMO comando del pipeline                */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_cmds(t_cmd *cmd)
{
	int	n;

	n = 0;
	while (cmd)
	{
		n++;
		cmd = cmd->next;
	}
	return (n);
}

/*
** exec_builtin_redir: Ejecuta un builtin aplicando redirecciones.
** Guarda los fd originales, aplica redirecciones, ejecuta, y restaura.
*/
static int	exec_builtin_redir(t_cmd *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirections(cmd->redirs) != 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	ret = exec_builtin(cmd, shell);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (ret);
}

/*
** exec_simple: Ejecuta un comando simple (sin pipeline).
** Si es builtin, lo ejecuta en el padre con redirecciones.
** Si no, fork + execve.
*/
static int	exec_simple(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**envp;

	if (is_builtin(cmd->argv[0]))
		return (exec_builtin_redir(cmd, shell));
	setup_signals_exec();
	pid = fork();
	if (pid == 0)
	{
		setup_signals_child();
		if (apply_redirections(cmd->redirs) != 0)
			exit(1);
		path = find_executable(cmd->argv[0], shell->env);
		if (!path)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
		envp = env_to_array(shell->env);
		execve(path, cmd->argv, envp);
		perror("minishell");
		free(path);
		free_str_array(envp);
		exit(126);
	}
	waitpid(pid, &status, 0);
	setup_signals_prompt();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

/*
** child_process: Logica del proceso hijo dentro de un pipeline.
** Configura los file descriptors para leer del pipe anterior
** y escribir al pipe siguiente, luego ejecuta el comando.
*/
static void	child_process(t_cmd *cmd, int prev_fd, int *pipefd,
		t_shell *shell)
{
	char	*path;
	char	**envp;

	setup_signals_child();
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next && pipefd)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (apply_redirections(cmd->redirs) != 0)
		exit(1);
	if (is_builtin(cmd->argv[0]))
		exit(exec_builtin(cmd, shell));
	path = find_executable(cmd->argv[0], shell->env);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	envp = env_to_array(shell->env);
	execve(path, cmd->argv, envp);
	perror("minishell");
	exit(126);
}

/*
** exec_pipeline: Ejecuta una cadena de comandos conectados por pipes.
** Crea un pipe entre cada par de comandos adyacentes.
*/
static int	exec_pipeline(t_cmd *cmd, t_shell *shell)
{
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	int		status;

	prev_fd = -1;
	status = 0;
	setup_signals_exec();
	while (cmd)
	{
		if (cmd->next)
			pipe(pipefd);
		pid = fork();
		if (pid == 0)
			child_process(cmd, prev_fd, cmd->next ? pipefd : NULL, shell);
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		cmd = cmd->next;
	}
	while (waitpid(-1, &status, 0) > 0)
		;
	setup_signals_prompt();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

/*
** executor: Punto de entrada. Decide si ejecutar como simple o pipeline.
*/
int	executor(t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (count_cmds(cmd) == 1)
		return (exec_simple(cmd, shell));
	return (exec_pipeline(cmd, shell));
}
