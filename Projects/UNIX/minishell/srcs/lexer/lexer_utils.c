/* ************************************************************************** */
/*                                                                            */
/*   lexer_utils.c - Utilidades auxiliares para el lexer                       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** is_operator_char: Verifica si un caracter es un operador del shell.
*/
int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';');
}

/*
** token_type_str: Devuelve el nombre del tipo de token (para debug).
*/
const char	*token_type_str(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	if (type == TOKEN_PIPE)
		return ("PIPE");
	if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == TOKEN_APPEND)
		return ("APPEND");
	if (type == TOKEN_SEMICOLON)
		return ("SEMICOLON");
	return ("UNKNOWN");
}
