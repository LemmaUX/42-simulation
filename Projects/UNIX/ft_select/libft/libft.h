#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <string.h>

size_t			ft_strlen(const char *s);
char			*ft_strcpy(char *dst, const char *src);
char			*ft_strdup(const char *s);
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memset(void *b, int c, size_t len);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strcat(char *s1, const char *s2);
int				ft_atoi(const char *str);
char			*ft_itoa(int n);
void			ft_putstr(const char *s);
void			ft_putchar(char c);

#endif
