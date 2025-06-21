#include "ft_printf.h"

// Esta función imprime una cadena de caracteres.
// Si la cadena es NULL, imprime "(null)".
// Retorna la cantidad de caracteres impresos.
int	ft_putstr(const char *s)
{
    int	count = 0;

    if (!s)
        return (write(1, "(null)", 6));

    while (s[count])
        count += write(1, &s[count], 1);

    return (count);
}

/*
Explicación paso a paso:
- Si el puntero recibido es NULL, imprime "(null)" para evitar errores y sigue el comportamiento estándar de printf.
- Si la cadena no es NULL, recorre cada carácter y lo imprime usando write.
- Suma y retorna la cantidad de caracteres impresos.
- Ejemplo de uso:
    ft_putstr("Hola");   // Imprime "Hola"
    ft_putstr(NULL);     // Imprime