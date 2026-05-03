int	ft_strcmp(char *s1, char *s2)
{
	unsigned char	*c1;
	unsigned char	*c2;

	c1 = (unsigned char *)s1;
	c2 = (unsigned char *)s2;
	while (*c1 && *c1 == *c2)
	{
		c1++;
		c2++;
	}
	return (*c1 - *c2);
}
