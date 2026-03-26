#include "libft.h"

char	*ft_strcpy(char *dst, const char *src)
{
	char	*ptr;

	ptr = dst;
	while (*src)
		*ptr++ = *src++;
	*ptr = '\0';
	return (dst);
}
