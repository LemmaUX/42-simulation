#include "ft_printf.h"

// Esta función imprime una dirección de puntero en formato hexadecimal con el prefijo "0x".
// Retorna la cantidad de caracteres impresos.
int	ft_putptr(void *ptr)
{
    int				count = 0;
    unsigned long	address;

    address = (unsigned long)ptr;

    // Imprime el prefijo "0x"
    count += ft_putstr("0x");

    // Si el puntero es NULL, imprime solo "0x0"
    if (address == 0)
        count += ft_putchar('0');
    else
    {
        // Imprime la dirección en hexadecimal (base 16)
        count += ft_putptr_hex(address);
    }
    return (count);
}

// Función auxiliar para imprimir un número largo en hexadecimal (solo para uso interno)
int	ft_putptr_hex(unsigned long n)
{
    int count = 0;
    if (n >= 16)
        count += ft_putptr_hex(n / 16);
    count += ft_putchar("0123456789abcdef"[n % 16]);
    return count;
}

/*
Explicación paso a paso:
- Convierte el puntero a un número largo sin signo (unsigned long) para poder imprimirlo como número.
- Imprime el prefijo "0x" que es estándar para direcciones en hexadecimal.
- Si el puntero es NULL, imprime "0x0".
- Si no, imprime la dirección en hexadecimal usando recursividad.
- Suma y retorna la cantidad de caracteres impresos.
- Ejemplo de uso:
    ft_putptr((void*)0x7ffeefbff5c8); // Imprime "0x7ffeefbff5c8"
    ft_putptr(NULL);                  // Imprime "0x0"