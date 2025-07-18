#include "ft_printf.h"

// Esta función imprime un número 'n' en la base indicada por la cadena 'base'.
// Por ejemplo, para hexadecimal usa "0123456789abcdef" como base.
// Retorna la cantidad de caracteres impresos.
int	ft_putnbr_base(unsigned int n, const char *base)
{
    int		count = 0;
    unsigned int	base_len = 0;

    // Calcula la longitud de la base (por ejemplo, 16 para hexadecimal)
    while (base[base_len])
        base_len++;

    // Si el número es mayor o igual a la base, imprime primero los dígitos más significativos
    if (n >= base_len)
        count += ft_putnbr_base(n / base_len, base);

    // Imprime el dígito menos significativo
    count += ft_putchar(base[n % base_len]);

    return (count);
}

/*
Explicación paso a paso:
- La función recibe un número 'n' y una cadena 'base' que representa los caracteres de la base.
- Calcula la longitud de la base (por ejemplo, 10 para decimal, 16 para hexadecimal).
- Si el número es mayor o igual a la base, llama recursivamente a sí misma para imprimir los dígitos más altos primero.
- Luego imprime el dígito correspondiente al resto de la división (n % base_len).
- Suma y retorna la cantidad de caracteres impresos.
- Ejemplo de uso:
    ft_putnbr_base(255, "0123456789abcdef"); // Imprime "ff"
    ft_putnbr_base(42, "01"); // Imprime "101010"
*/