/* ************************************************************************** */
/*                                                                            */
/*   lexer.c - Analisis lexico (tokenizacion)                                 */
/*                                                                            */
/*   El lexer toma el string crudo del input y lo rompe en tokens.            */
/*   Usa una maquina de estados finitos (FSM) para manejar comillas:          */
/*                                                                            */
/*     STATE_DEFAULT  -> caracteres normales                                  */
/*     STATE_SQUOTE   -> dentro de comillas simples (todo literal)            */
/*     STATE_DQUOTE   -> dentro de comillas dobles ($ se expande)             */
/*                                                                            */
/*   Los operadores (|, <, >, >>, ;) se emiten como tokens separados.         */
/*   Los espacios separan tokens WORD.                                        */
/*   Las comillas NO se incluyen en el valor del token, solo agrupan.         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*token_new(char *value, t_token_type type)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = value;
	tok->type = type;
	tok->next = NULL;
	return (tok);
}

static void	token_add(t_token **head, t_token **tail, t_token *new)
{
	if (!*head)
		*head = new;
	else
		(*tail)->next = new;
	*tail = new;
}

void	token_free(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

/*
** emit_operator: Emite un token de operador (|, <, >, >>, ;).
** Avanza el indice segun la longitud del operador.
*/
static t_token	*emit_operator(const char *input, int *i)
{
	t_token_type	type;
	char			*val;

	if (input[*i] == '|')
		type = TOKEN_PIPE;
	else if (input[*i] == ';')
		type = TOKEN_SEMICOLON;
	else if (input[*i] == '<')
		type = TOKEN_REDIR_IN;
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		val = ft_strdup(">>");
		(*i) += 2;
		return (token_new(val, TOKEN_APPEND));
	}
	else
		type = TOKEN_REDIR_OUT;
	val = ft_strndup(&input[*i], 1);
	(*i)++;
	return (token_new(val, type));
}

/*
** read_word: Lee un token WORD.
** Maneja comillas simples y dobles como parte del mismo token.
** Las comillas se CONSERVAN en esta fase (se eliminan despues en el expander).
*/
static t_token	*read_word(const char *input, int *i)
{
	int		start;
	char	quote;

	start = *i;
	while (input[*i])
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			quote = input[*i];
			(*i)++;
			while (input[*i] && input[*i] != quote)
				(*i)++;
			if (input[*i] == quote)
				(*i)++;
		}
		else if (ft_isspace(input[*i]) || input[*i] == '|'
			|| input[*i] == '<' || input[*i] == '>'
			|| input[*i] == ';')
			break ;
		else
			(*i)++;
	}
	return (token_new(ft_strndup(input + start, *i - start), TOKEN_WORD));
}

/*
** lexer: Punto de entrada. Recorre el input y genera la lista de tokens.
*/
t_token	*lexer(const char *input)
{
	t_token	*head;
	t_token	*tail;
	t_token	*tok;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		if (input[i] == '|' || input[i] == '<'
			|| input[i] == '>' || input[i] == ';')
			tok = emit_operator(input, &i);
		else
			tok = read_word(input, &i);
		if (tok)
			token_add(&head, &tail, tok);
	}
	return (head);
}
