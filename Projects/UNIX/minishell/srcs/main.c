/* ************************************************************************** */
/*                                                                            */
/*   main.c - Punto de entrada de minishell                                   */
/*                                                                            */
/*   El corazon del shell es un loop REPL:                                    */
/*     1. READ   - Leer input del usuario (prompt)                            */
/*     2. EVAL   - Lexer -> Parser -> Expander                                */
/*     3. EXEC   - Ejecutar los comandos                                      */
/*     4. PRINT  - El output lo hacen los procesos hijos                      */
/*     5. LOOP   - Volver al paso 1                                           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** read_line: Lee una linea de stdin.
** Muestra el prompt y lee caracter por caracter hasta '\n' o EOF.
** Retorna NULL si se recibe EOF (ctrl-D).
*/
static char	*read_line(void)
{
	char	*line;
	char	buf[2];
	char	*tmp;
	int		ret;

	line = ft_strdup("");
	if (!line)
		return (NULL);
	buf[1] = '\0';
	ft_putstr_fd(PROMPT, STDERR_FILENO);
	while (1)
	{
		ret = read(STDIN_FILENO, buf, 1);
		if (ret <= 0)
		{
			if (ft_strlen(line) == 0)
			{
				free(line);
				return (NULL);
			}
			return (line);
		}
		if (buf[0] == '\n')
			return (line);
		tmp = ft_strjoin(line, buf);
		free(line);
		line = tmp;
		if (!line)
			return (NULL);
	}
}

/*
** process_line: Toma una linea y ejecuta las pipelines.
** Los ';' separan pipelines independientes. Para cada grupo de tokens
** entre ';', se invoca parser + executor por separado.
*/
static void	process_line(char *line, t_shell *shell)
{
	t_token	*tokens;
	t_token	*cur;
	t_token	*seg_start;
	t_token	*prev;
	t_cmd	*cmds;

	if (ft_strlen(line) == 0)
		return ;
	tokens = lexer(line);
	if (!tokens)
		return ;
	seg_start = tokens;
	while (seg_start && shell->running)
	{
		cur = seg_start;
		prev = NULL;
		while (cur && cur->type != TOKEN_SEMICOLON)
		{
			prev = cur;
			cur = cur->next;
		}
		if (prev)
			prev->next = NULL;
		else if (cur && cur->type == TOKEN_SEMICOLON)
		{
			seg_start = cur->next;
			continue ;
		}
		cmds = parser(seg_start, shell);
		if (cmds)
		{
			shell->exit_status = executor(cmds, shell);
			cmd_free(cmds);
		}
		if (prev)
			prev->next = cur;
		if (cur)
			seg_start = cur->next;
		else
			seg_start = NULL;
	}
	token_free(tokens);
}

/*
** main: Inicializa el shell y entra en el loop REPL.
**
** argc/argv no se usan (minishell no recibe argumentos).
** envp es el environment heredado del proceso padre.
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*line;

	(void)argc;
	(void)argv;
	shell.env = env_init(envp);
	shell.exit_status = 0;
	shell.running = 1;
	setup_signals_prompt();
	while (shell.running)
	{
		line = read_line();
		if (!line)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			break ;
		}
		process_line(line, &shell);
		free(line);
	}
	env_free(shell.env);
	return (shell.exit_status);
}
