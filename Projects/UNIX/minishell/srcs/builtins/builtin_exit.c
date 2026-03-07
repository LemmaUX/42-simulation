/* ************************************************************************** */
/*                                                                            */
/*   builtin_exit.c - Implementacion de exit                                  */
/*                                                                            */
/*   exit [n]                                                                 */
/*     - Sin argumentos: sale con el ultimo exit status                       */
/*     - Con argumento numerico: sale con ese codigo                          */
/*     - Con argumento no numerico: "numeric argument required", sale con 255 */
/*     - Con mas de un argumento: "too many arguments", NO sale               */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_cmd *cmd, t_shell *shell)
{
	int	exit_code;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (cmd->argc == 1)
	{
		shell->running = 0;
		return (shell->exit_status);
	}
	if (!is_numeric(cmd->argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		shell->running = 0;
		return (255);
	}
	if (cmd->argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	exit_code = ft_atoi(cmd->argv[1]);
	shell->running = 0;
	return (exit_code % 256);
}
