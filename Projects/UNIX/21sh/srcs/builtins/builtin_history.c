/* ************************************************************************** */
/*                                                                            */
/*   builtin_history.c - Muestra el historial de comandos                     */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int	builtin_history(t_shell *shell)
{
	t_history	*cur;
	int			num;

	(void)shell;
	cur = shell->history;
	num = 1;
	while (cur)
	{
		ft_putnbr_fd(num, STDOUT_FILENO);
		ft_putstr_fd("  ", STDOUT_FILENO);
		ft_putstr_fd(cur->line, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		cur = cur->next;
		num++;
	}
	return (0);
}
