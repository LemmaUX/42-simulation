#include "ft_printf.h"

/*
** ft_putnbr - Imprime un entero con signo en formato decimal.
**
** Relación con el proyecto:
**   Maneja los especificadores '%d' e '%i' de ft_printf.
**   Ambos especificadores imprimen enteros decimales con signo (son equivalentes
**   para printf en el caso de output).
**
** ¿Por qué el caso especial de -2147483648 (INT_MIN)?
**   En C, el rango de int es [-2147483648, 2147483647].
**   Si hacemos n = -n con n == -2147483648, obtenemos overflow:
**   el resultado no cabe en un int (2147483648 > INT_MAX).
**   Solución: imprimirlo directamente como string.
**
** Algoritmo de recursión:
**   Para imprimir 1234:
**     ft_putnbr(1234) → ft_putnbr(123) → ft_putnbr(12) → ft_putnbr(1)
**     luego imprime: '1', '2', '3', '4'  (orden correcto)
**
** Retorna: número de caracteres impresos.
*/
int	ft_putnbr(int n)
{
	int	count;

	count = 0;
	if (n == -2147483648)
		return (ft_putstr("-2147483648"));
	if (n < 0)
	{
		count += ft_putchar('-');
		n = -n;
	}
	if (n >= 10)
		count += ft_putnbr(n / 10);
	count += ft_putchar((n % 10) + '0');
	return (count);
}
