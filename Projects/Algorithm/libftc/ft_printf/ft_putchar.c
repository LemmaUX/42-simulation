#include "ft_printf.h"

/*
** ft_putchar - Imprime un único carácter en la salida estándar.
**
** Relación con el proyecto:
**   Es la función más básica de ft_printf. Todos los demás helpers
**   (ft_putstr, ft_putnbr, ft_putnbr_base, ft_putptr) terminan llamando
**   a esta función para imprimir carácter a carácter.
**   Equivale al comportamiento de printf con el especificador '%c'.
**
** Por qué usamos write() en lugar de putchar():
**   El proyecto prohíbe funciones de stdio como putchar/puts.
**   write(1, ...) escribe directamente en el file descriptor 1 (stdout).
**
** Retorna: el número de bytes escritos (siempre 1 si no hay error).
*/
int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}
