#include "ft_printf.h"

/*
** ft_putstr - Imprime una cadena de caracteres en la salida estándar.
**
** Relación con el proyecto:
**   Maneja el especificador '%s' de ft_printf.
**   El printf real imprime "(null)" cuando recibe un NULL → nosotros también.
**
** ¿Por qué comprobamos s == NULL?
**   Si pasamos NULL a write() causaría comportamiento indefinido (segfault).
**   El printf real imprime "(null)" en ese caso, lo replicamos.
**
** ¿Por qué no usamos strlen?
**   El subject permite libft, pero aquí implementamos el conteo directamente
**   para no depender de otra función y tener control total del count.
**
** Retorna: número de caracteres impresos.
*/
int	ft_putstr(const char *s)
{
	int	count;

	count = 0;
	if (!s)
		return (write(1, "(null)", 6));
	while (s[count])
	{
		write(1, &s[count], 1);
		count++;
	}
	return (count);
}
