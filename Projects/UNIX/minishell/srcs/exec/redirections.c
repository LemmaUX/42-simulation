/* ************************************************************************** */
/*                                                                            */
/*   redirections.c - Aplicar redirecciones de archivos                       */
/*                                                                            */
/*   Las redirecciones se aplican en el proceso HIJO, antes de execve.        */
/*   Se recorre la lista de t_redir y para cada una:                          */
/*     <   -> abrir archivo para lectura, dup2 a STDIN                        */
/*     >   -> abrir/crear/truncar archivo, dup2 a STDOUT                      */
/*     >>  -> abrir/crear/append archivo, dup2 a STDOUT                       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** apply_redirections: Aplica todas las redirecciones de un comando.
** Retorna 0 si OK, 1 si hubo error (archivo no existe, permisos, etc).
*/
int	apply_redirections(t_redir *redir)
{
	int	fd;

	while (redir)
	{
		if (redir->type == TOKEN_REDIR_IN)
			fd = open(redir->file, O_RDONLY);
		else if (redir->type == TOKEN_REDIR_OUT)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == TOKEN_APPEND)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
		{
			redir = redir->next;
			continue ;
		}
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(redir->file);
			return (1);
		}
		if (redir->type == TOKEN_REDIR_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}
