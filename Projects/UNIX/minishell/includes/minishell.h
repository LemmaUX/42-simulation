/* ************************************************************************** */
/*                                                                            */
/*   minishell.h                                                              */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <errno.h>
# include <string.h>

# define PROMPT "minishell$ "

/* ─── Token types ─────────────────────────────────────────────────────────── */

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_SEMICOLON
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/* ─── Redirections ────────────────────────────────────────────────────────── */

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

/* ─── Command (one node in a pipeline) ────────────────────────────────────── */

typedef struct s_cmd
{
	char			**argv;
	int				argc;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

/* ─── Environment variable ────────────────────────────────────────────────── */

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* ─── Shell state ─────────────────────────────────────────────────────────── */

typedef struct s_shell
{
	t_env	*env;
	int		exit_status;
	int		running;
}	t_shell;

/* ─── Environment (srcs/env/) ─────────────────────────────────────────────── */

t_env	*env_init(char **envp);
char	*env_get(t_env *env, const char *key);
int		env_set(t_env **env, const char *key, const char *value);
int		env_unset(t_env **env, const char *key);
void	env_free(t_env *env);
char	**env_to_array(t_env *env);
void	free_str_array(char **arr);

/* ─── Lexer (srcs/lexer/) ─────────────────────────────────────────────────── */

t_token	*lexer(const char *input);
void	token_free(t_token *tokens);

/* ─── Parser (srcs/parser/) ───────────────────────────────────────────────── */

t_cmd	*parser(t_token *tokens, t_shell *shell);
void	cmd_free(t_cmd *cmd);

/* ─── Expander (srcs/parser/) ─────────────────────────────────────────────── */

char	*expand_variables(const char *str, t_shell *shell);

/* ─── Executor (srcs/exec/) ───────────────────────────────────────────────── */

int		executor(t_cmd *cmd, t_shell *shell);
char	*find_executable(const char *cmd, t_env *env);
int		apply_redirections(t_redir *redir);

/* ─── Builtins (srcs/builtins/) ───────────────────────────────────────────── */

int		is_builtin(const char *cmd);
int		exec_builtin(t_cmd *cmd, t_shell *shell);
int		builtin_echo(t_cmd *cmd);
int		builtin_cd(t_cmd *cmd, t_shell *shell);
int		builtin_pwd(void);
int		builtin_export(t_cmd *cmd, t_shell *shell);
int		builtin_unset(t_cmd *cmd, t_shell *shell);
int		builtin_env(t_shell *shell);
int		builtin_exit(t_cmd *cmd, t_shell *shell);

/* ─── Signals (srcs/signals/) ─────────────────────────────────────────────── */

void	setup_signals_prompt(void);
void	setup_signals_exec(void);
void	setup_signals_child(void);

#endif
