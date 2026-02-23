#include "ft_printf.h"

/*
** ft_putptr_hex - Helper interno para imprimir un unsigned long en hex.
**
** Solo se usa desde ft_putptr(). Está en el header pero es de uso interno.
** Utiliza unsigned long porque las direcciones de memoria en sistemas
** de 64 bits no caben en un unsigned int (son de 64 bits).
*/
int	ft_putptr_hex(unsigned long n)
{
	int	count;

	count = 0;
	if (n >= 16)
		count += ft_putptr_hex(n / 16);
	count += ft_putchar("0123456789abcdef"[n % 16]);
	return (count);
}

/*
** ft_putptr - Imprime una dirección de puntero en formato hexadecimal.
**
** Relación con el proyecto:
**   Maneja el especificador '%p' de ft_printf.
**   El formato estándar es "0x" seguido de la dirección en hex minúsculas.
**
** ¿Por qué casteamos a unsigned long?
**   Un puntero (void *) no es un número, no podemos operar aritmética sobre él.
**   Al castear a unsigned long obtenemos el valor numérico de la dirección,
**   que tiene el mismo tamaño que un puntero en sistemas de 64 bits.
**
** Casos borde:
**   NULL → imprime "0x0" (el printf real imprime "(nil)", pero en 42 se acepta "0x0").
**
** Retorna: número de caracteres impresos.
*/
int	ft_putptr(void *ptr)
{
	int				count;
	unsigned long	address;

	count = 0;
	address = (unsigned long)ptr;
	count += ft_putstr("0x");
	if (address == 0)
		count += ft_putchar('0');
	else
		count += ft_putptr_hex(address);
	return (count);
}
