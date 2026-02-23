#include "ft_printf.h"

/*
** ft_putnbr_base - Imprime un número en la base dada como string.
**
** Relación con el proyecto:
**   Maneja los especificadores '%u', '%x' y '%X':
**     %u → base "0123456789"         (decimal sin signo)
**     %x → base "0123456789abcdef"   (hexadecimal minúsculas)
**     %X → base "0123456789ABCDEF"   (hexadecimal mayúsculas)
**
** ¿Por qué unsigned int y no int?
**   %u, %x, %X nunca tienen signo negativo. Un int negativo pasado como
**   unsigned int se interpreta como su equivalente en complemento a 2,
**   que es el comportamiento correcto del printf real.
**
** ¿Cómo funciona la base?
**   La longitud de la cadena 'base' es el valor numérico de la base.
**   'base[n % base_len]' mapea el resto a su carácter representativo.
**   Ejemplo con base 16 y n=255:
**     255 / 16 = 15 → recurse → base[15] = 'f'
**     255 % 16 = 15            → base[15] = 'f'
**     Resultado: "ff"
**
** Retorna: número de caracteres impresos.
*/
int	ft_putnbr_base(unsigned int n, const char *base)
{
	int			count;
	unsigned int	base_len;

	count = 0;
	base_len = 0;
	while (base[base_len])
		base_len++;
	if (n >= base_len)
		count += ft_putnbr_base(n / base_len, base);
	count += ft_putchar(base[n % base_len]);
	return (count);
}
