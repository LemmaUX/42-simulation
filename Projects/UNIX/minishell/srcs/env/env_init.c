/* ************************************************************************** */
/*                                                                            */
/*   env_init.c - Copia el environment del sistema a nuestra lista enlazada   */
/*                                                                            */
/*   Por que copiar el environment?                                           */
/*   El char **envp que recibe main() es un puntero a la memoria del          */
/*   proceso padre. Si modificamos esa memoria, corrompemos datos.            */
/*   Ademas, necesitamos poder anadir/eliminar variables (export/unset).      */
/*   Una lista enlazada nos da esa flexibilidad.                              */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Crea un nodo t_env a partir de un string "KEY=VALUE".
** Si no hay '=', el valor es NULL (variable sin valor asignado).
*/
static t_env	*env_new_node(const char *str)
{
	t_env	*node;
	char	*eq;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	eq = ft_strchr(str, '=');
	if (eq)
	{
		node->key = ft_strndup(str, eq - str);
		node->value = ft_strdup(eq + 1);
	}
	else
	{
		node->key = ft_strdup(str);
		node->value = NULL;
	}
	node->next = NULL;
	if (!node->key)
	{
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

/*
** Recorre envp[] y construye la lista enlazada.
** Cada entrada de envp tiene formato "KEY=VALUE".
*/
t_env	*env_init(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*node;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (envp && envp[i])
	{
		node = env_new_node(envp[i]);
		if (!node)
			break ;
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		i++;
	}
	return (head);
}
