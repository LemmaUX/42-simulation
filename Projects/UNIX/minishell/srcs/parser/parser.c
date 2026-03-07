/* ************************************************************************** */
/*                                                                            */
/*   parser.c - Analisis sintactico                                           */
/*                                                                            */
/*   El parser toma la lista de tokens del lexer y construye una              */
/*   estructura de comandos (lista enlazada de t_cmd).                        */
/*                                                                            */
/*   Cada t_cmd contiene:                                                     */
/*     - argv: array de argumentos (como lo espera execve)                    */
/*     - redirs: lista de redirecciones asociadas al comando                  */
/*     - next: siguiente comando en el pipeline (separado por |)             */
/*                                                                            */
/*   Los pipelines se separan por ';'.                                        */
/*   La expansion de variables se hace aqui, llamando a expand_variables().   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redir_add(t_redir **head, t_token_type type, char *file)
{
	t_redir	*new;
	t_redir	*cur;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	new->type = type;
	new->file = ft_strdup(file);
	new->next = NULL;
	if (!*head)
		*head = new;
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
}

static void	redir_free(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->file);
		free(redir);
		redir = tmp;
	}
}

void	cmd_free(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->argv)
		{
			i = 0;
			while (cmd->argv[i])
				free(cmd->argv[i++]);
			free(cmd->argv);
		}
		redir_free(cmd->redirs);
		free(cmd);
		cmd = tmp;
	}
}

/*
** count_args: Cuenta cuantos TOKEN_WORD hay antes del siguiente
** operador pipe/semicolon (para saber el tamano de argv).
*/
static int	count_args(t_token *tok)
{
	int	count;

	count = 0;
	while (tok && tok->type != TOKEN_PIPE && tok->type != TOKEN_SEMICOLON)
	{
		if (tok->type == TOKEN_WORD)
			count++;
		else if (tok->type == TOKEN_REDIR_IN || tok->type == TOKEN_REDIR_OUT
			|| tok->type == TOKEN_APPEND)
		{
			tok = tok->next;
			if (tok)
				tok = tok->next;
			continue ;
		}
		tok = tok->next;
	}
	return (count);
}

/*
** parse_syntax_error: Validaciones basicas de sintaxis.
** Retorna 1 si hay error, 0 si todo esta bien.
*/
static int	parse_syntax_error(t_token *tokens)
{
	t_token	*tok;

	if (tokens && tokens->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	tok = tokens;
	while (tok)
	{
		if ((tok->type == TOKEN_REDIR_IN || tok->type == TOKEN_REDIR_OUT
				|| tok->type == TOKEN_APPEND)
			&& (!tok->next || tok->next->type != TOKEN_WORD))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
			return (1);
		}
		if (tok->type == TOKEN_PIPE
			&& (!tok->next || tok->next->type == TOKEN_PIPE))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
			return (1);
		}
		tok = tok->next;
	}
	return (0);
}

/*
** parse_cmd: Parsea un solo comando (hasta pipe o semicolon).
** Llena argv y redirecciones. Avanza *tok al siguiente token tras el comando.
*/
static t_cmd	*parse_cmd(t_token **tok, t_shell *shell)
{
	t_cmd			*cmd;
	int				argc;
	char			*expanded;
	t_token_type	redir_type;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	argc = count_args(*tok);
	cmd->argv = ft_calloc(argc + 1, sizeof(char *));
	if (!cmd->argv)
		return (free(cmd), NULL);
	argc = 0;
	while (*tok && (*tok)->type != TOKEN_PIPE && (*tok)->type != TOKEN_SEMICOLON)
	{
		if ((*tok)->type == TOKEN_REDIR_IN || (*tok)->type == TOKEN_REDIR_OUT
			|| (*tok)->type == TOKEN_APPEND)
		{
			redir_type = (*tok)->type;
			*tok = (*tok)->next;
			if (*tok)
			{
				expanded = expand_variables((*tok)->value, shell);
				redir_add(&cmd->redirs, redir_type, expanded);
				free(expanded);
			}
		}
		else if ((*tok)->type == TOKEN_WORD)
		{
			expanded = expand_variables((*tok)->value, shell);
			cmd->argv[argc++] = expanded;
		}
		if (*tok)
			*tok = (*tok)->next;
	}
	cmd->argc = argc;
	return (cmd);
}

/*
** parser: Punto de entrada del parser.
** Recorre los tokens, genera una lista enlazada de t_cmd.
** Los pipes conectan comandos (cmd->next),
** los semicolons separan pipelines independientes.
*/
t_cmd	*parser(t_token *tokens, t_shell *shell)
{
	t_cmd	*head;
	t_cmd	*tail;
	t_cmd	*cmd;

	if (parse_syntax_error(tokens))
	{
		shell->exit_status = 258;
		return (NULL);
	}
	head = NULL;
	tail = NULL;
	while (tokens)
	{
		cmd = parse_cmd(&tokens, shell);
		if (cmd && cmd->argc > 0)
		{
			if (!head)
				head = cmd;
			else
				tail->next = cmd;
			tail = cmd;
		}
		else
			cmd_free(cmd);
		if (tokens && (tokens->type == TOKEN_PIPE
				|| tokens->type == TOKEN_SEMICOLON))
			tokens = tokens->next;
	}
	return (head);
}
