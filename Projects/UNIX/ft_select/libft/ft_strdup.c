#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(s) + 1;
	dup = (char *)malloc(len);
	if (!dup)
		return (NULL);
	ft_strcpy(dup, s);
	return (dup);
}
