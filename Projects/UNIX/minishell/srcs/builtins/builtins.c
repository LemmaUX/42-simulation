/* ************************************************************************** */
/*                                                                            */
/*   builtins.c - Dispatcher de builtins                                      */
/*                                                                            */
/*   Los builtins son comandos que el shell ejecuta internamente,             */
/*   sin crear un proceso hijo (fork). Esto es necesario porque               */
/*   algunos builtins modifican el estado del shell:                          */
/*     - cd: cambia el directorio del proceso (chdir)                         */
/*     - export: modifica el environment del shell                            */
/*     - unset: elimina variables del environment                             */
/*     - exit: termina el shell                                               */
/*                                                                            */
/*   Si estos se ejecutaran en un fork, los cambios se perderian              */
/*   al terminar el proceso hijo.                                             */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

int	exec_builtin(t_cmd *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (builtin_echo(cmd));
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (builtin_cd(cmd, shell));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(cmd, shell));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(cmd, shell));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (builtin_env(shell));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (builtin_exit(cmd, shell));
	return (1);
}
