/* ************************************************************************** */
/*                                                                            */
/*   expander.c - Expansion de variables y eliminacion de comillas            */
/*                                                                            */
/*   Recorre un string caracter por caracter:                                 */
/*     - Dentro de comillas simples: todo es literal                          */
/*     - Dentro de comillas dobles: se expanden $VAR                          */
/*     - Fuera de comillas: se expanden $VAR                                  */
/*     - $? se reemplaza por el exit status del ultimo comando                */
/*                                                                            */
/*   Las comillas se eliminan del resultado final.                            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_char(char *result, char c)
{
	char	buf[2];
	char	*new;

	buf[0] = c;
	buf[1] = '\0';
	new = ft_strjoin(result, buf);
	free(result);
	return (new);
}

static char	*append_str(char *result, const char *s)
{
	char	*new;

	if (!s)
		return (result);
	new = ft_strjoin(result, s);
	free(result);
	return (new);
}

/*
** expand_var_name: Lee el nombre de la variable a partir de la posicion actual.
** El nombre puede contener letras, digitos y '_'.
** Avanza *i hasta el caracter despues del nombre.
*/
static char	*expand_var(const char *str, int *i, t_shell *shell)
{
	int		start;
	char	*key;
	char	*value;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	if (!ft_isalpha(str[*i]) && str[*i] != '_')
		return (ft_strdup("$"));
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	key = ft_strndup(str + start, *i - start);
	value = env_get(shell->env, key);
	free(key);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

/*
** expand_variables: Procesa un token WORD completo.
** Maneja comillas y expansion de variables.
*/
char	*expand_variables(const char *str, t_shell *shell)
{
	char	*result;
	char	*var;
	int		i;
	int		in_sq;
	int		in_dq;

	result = ft_strdup("");
	i = 0;
	in_sq = 0;
	in_dq = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_dq)
		{
			in_sq = !in_sq;
			i++;
		}
		else if (str[i] == '"' && !in_sq)
		{
			in_dq = !in_dq;
			i++;
		}
		else if (str[i] == '$' && !in_sq && str[i + 1]
			&& !ft_isspace(str[i + 1]))
		{
			var = expand_var(str, &i, shell);
			result = append_str(result, var);
			free(var);
		}
		else
			result = append_char(result, str[i++]);
	}
	return (result);
}
