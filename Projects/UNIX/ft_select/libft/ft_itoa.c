#include "libft.h"

char	*ft_itoa(int n)
{
	char	*result;
	int		len;
	int		temp;
	int		sign;

	sign = (n < 0) ? -1 : 1;
	len = 0;
	temp = n;
	while (temp)
	{
		len++;
		temp /= 10;
	}
	if (n <= 0)
		len++;
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	result[len] = '\0';
	if (n == 0)
		result[0] = '0';
	else
	{
		n = n * sign;
		while (n)
		{
			result[--len] = (n % 10) + '0';
			n /= 10;
		}
		if (sign == -1)
			result[0] = '-';
	}
	return (result);
}
