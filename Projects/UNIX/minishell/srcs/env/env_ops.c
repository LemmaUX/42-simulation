/* ************************************************************************** */
/*                                                                            */
/*   env_ops.c - Operaciones sobre el environment (get, set, unset)           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** env_get: Busca una variable por su key y retorna el valor.
** Retorna NULL si no existe.
*/
char	*env_get(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/*
** env_set: Establece KEY=VALUE en el environment.
** Si la key ya existe, reemplaza el valor.
** Si no existe, crea un nuevo nodo al final de la lista.
*/
int	env_set(t_env **env, const char *key, const char *value)
{
	t_env	*cur;
	t_env	*node;

	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			cur->value = value ? ft_strdup(value) : NULL;
			return (0);
		}
		cur = cur->next;
	}
	node = malloc(sizeof(t_env));
	if (!node)
		return (1);
	node->key = ft_strdup(key);
	node->value = value ? ft_strdup(value) : NULL;
	node->next = *env;
	*env = node;
	return (0);
}

/*
** env_unset: Elimina una variable del environment.
** Busca el nodo con la key correspondiente y lo libera.
*/
int	env_unset(t_env **env, const char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return (0);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}
