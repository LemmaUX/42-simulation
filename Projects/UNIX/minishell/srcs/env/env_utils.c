/* ************************************************************************** */
/*                                                                            */
/*   env_utils.c - Utilidades: liberar env, convertir a char**                */
/*                                                                            */
/*   env_to_array() convierte la lista enlazada a char** para pasarlo         */
/*   a execve(). execve requiere char *const envp[].                          */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_free(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

static int	env_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->value)
			count++;
		env = env->next;
	}
	return (count);
}

/*
** Genera "KEY=VALUE" para cada variable con valor asignado.
*/
char	**env_to_array(t_env *env)
{
	char	**arr;
	char	*tmp;
	int		i;

	arr = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value)
		{
			tmp = ft_strjoin(env->key, "=");
			arr[i] = ft_strjoin(tmp, env->value);
			free(tmp);
			i++;
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
