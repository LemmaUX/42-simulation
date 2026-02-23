#include "ft_printf.h"
#include <stdarg.h>

/*
** handle_format - Despachador de especificadores de formato.
**
** ¿Por qué existe esta función?
**   Separar la lógica de detección de especificadores del main loop
**   hace el código más limpio y extensible (principio de responsabilidad única).
**   ft_printf() se encarga del recorrido; handle_format() del "qué hacer".
**
** Especificadores del subject:
**   %c  → carácter            → ft_putchar(va_arg(..., int))
**   %s  → string              → ft_putstr(va_arg(..., char *))
**   %p  → puntero (void *)    → ft_putptr(va_arg(..., void *))
**   %d  → entero decimal      → ft_putnbr(va_arg(..., int))
**   %i  → entero decimal      → ft_putnbr(va_arg(..., int))
**   %u  → entero sin signo    → ft_putnbr_base(..., "0-9")
**   %x  → hexadecimal (lower) → ft_putnbr_base(..., "0-9a-f")
**   %X  → hexadecimal (upper) → ft_putnbr_base(..., "0-9A-F")
**   %%  → imprime '%' literal → ft_putchar('%')
**
** ¿Por qué %c usa va_arg(..., int)?
**   Por promoción de argumentos: en variádicos, los tipos más pequeños que int
**   (char, short) se promueven automáticamente a int. Leerlos como char sería UB.
**
** Retorna: número de caracteres impresos por el helper invocado.
*/
int	handle_format(char specifier, va_list args)
{
	int	count;

	count = 0;
	if (specifier == 'c')
		count = ft_putchar(va_arg(args, int));
	else if (specifier == 's')
		count = ft_putstr(va_arg(args, char *));
	else if (specifier == 'p')
		count = ft_putptr(va_arg(args, void *));
	else if (specifier == 'd' || specifier == 'i')
		count = ft_putnbr(va_arg(args, int));
	else if (specifier == 'u')
		count = ft_putnbr_base(va_arg(args, unsigned int), "0123456789");
	else if (specifier == 'x')
		count = ft_putnbr_base(va_arg(args, unsigned int), "0123456789abcdef");
	else if (specifier == 'X')
		count = ft_putnbr_base(va_arg(args, unsigned int), "0123456789ABCDEF");
	else if (specifier == '%')
		count = ft_putchar('%');
	return (count);
}
