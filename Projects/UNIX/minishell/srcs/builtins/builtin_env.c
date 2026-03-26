/* ************************************************************************** */
/*                                                                            */
/*   builtin_env.c - Implementacion de export, unset, env                     */
/*                                                                            */
/*   export [KEY=VALUE]                                                       */
/*     - Sin args: imprime todas las variables (declare -x KEY="VALUE")       */
/*     - Con args: establece la variable en el environment                    */
/*                                                                            */
/*   unset KEY [KEY ...]                                                      */
/*     - Elimina la(s) variable(s) del environment                            */
/*                                                                            */
/*   env                                                                      */
/*     - Imprime todas las variables con valor (KEY=VALUE)                    */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	export_print_all(t_env *env)
{
	while (env)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env->key, STDOUT_FILENO);
		if (env->value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(env->value, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putchar_fd('\n', STDOUT_FILENO);
		env = env->next;
	}
}

int	builtin_export(t_cmd *cmd, t_shell *shell)
{
	int		i;
	char	*eq;
	char	*key;
	int		ret;

	if (cmd->argc == 1)
	{
		export_print_all(shell->env);
		return (0);
	}
	ret = 0;
	i = 1;
	while (cmd->argv[i])
	{
		if (!is_valid_identifier(cmd->argv[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(cmd->argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
		}
		else
		{
			eq = ft_strchr(cmd->argv[i], '=');
			if (eq)
			{
				key = ft_strndup(cmd->argv[i], eq - cmd->argv[i]);
				env_set(&shell->env, key, eq + 1);
				free(key);
			}
			else
				env_set(&shell->env, cmd->argv[i], NULL);
		}
		i++;
	}
	return (ret);
}

int	builtin_unset(t_cmd *cmd, t_shell *shell)
{
	int	i;
	int	ret;

	ret = 0;
	i = 1;
	while (cmd->argv[i])
	{
		if (!is_valid_identifier(cmd->argv[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(cmd->argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
		}
		else
			env_unset(&shell->env, cmd->argv[i]);
		i++;
	}
	return (ret);
}

int	builtin_env(t_shell *shell)
{
	t_env	*env;

	env = shell->env;
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd(env->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(env->value, STDOUT_FILENO);
		}
		env = env->next;
	}
	return (0);
}
