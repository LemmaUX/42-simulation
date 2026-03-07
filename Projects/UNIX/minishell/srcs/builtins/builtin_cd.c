/* ************************************************************************** */
/*                                                                            */
/*   builtin_cd.c - Implementacion de cd y pwd                                */
/*                                                                            */
/*   cd [path]                                                                */
/*     - Sin argumentos: ir a $HOME                                           */
/*     - Con argumento: chdir(path)                                           */
/*     - Actualizar $OLDPWD y $PWD                                            */
/*                                                                            */
/*   pwd                                                                      */
/*     - Imprime el directorio actual con getcwd()                            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	*oldpwd;
	char	cwd[4096];

	if (cmd->argc > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (cmd->argc == 1)
	{
		path = env_get(shell->env, "HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else
		path = cmd->argv[1];
	oldpwd = getcwd(cwd, sizeof(cwd));
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	if (oldpwd)
		env_set(&shell->env, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
		env_set(&shell->env, "PWD", cwd);
	return (0);
}

int	builtin_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (0);
	}
	perror("minishell: pwd");
	return (1);
}
