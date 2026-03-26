#include "libft.h"

void	ft_putstr(const char *s)
{
	if (!s)
		return ;
	while (*s)
		ft_putchar(*s++);
}
