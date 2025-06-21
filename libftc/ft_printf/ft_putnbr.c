#include "ft_printf.h"

// Esta función imprime un número entero (int) en formato decimal.
// Retorna la cantidad de caracteres impresos.
int	ft_putnbr(int n)
{
    int	count = 0;

    // Maneja el caso especial del número más negativo (-2147483648)
    if (n == -2147483648)
        return (ft_putstr("-2147483648"));

    // Si el número es negativo, imprime el signo y convierte a positivo
    if (n < 0)
    {
        count += ft_putchar('-');
        n = -n;
    }

    // Si el número tiene más de un dígito, imprime primero los dígitos más significativos
    if (n >= 10)
        count += ft_putnbr(n / 10);

    // Imprime el dígito menos significativo
    count += ft_putchar((n % 10) + '0');

    return (count);
}

/*
Explicación paso a paso:
- Si el número es el más negativo posible, lo imprime directamente (por límites de representación en C).
- Si el número es negativo, imprime el signo '-' y lo convierte a positivo.
- Si el número tiene más de un dígito, llama recursivamente para imprimir los dígitos más altos primero.
- Finalmente, imprime el último dígito.
- Suma y retorna la cantidad de caracteres impresos.
- Ejemplo de uso:
    ft_putnbr(1234);   // Imprime "1234"
    ft_putnbr(-56);    // Imprime "-56"