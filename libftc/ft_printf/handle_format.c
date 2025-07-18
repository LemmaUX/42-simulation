#include "ft_printf.h"
#include <stdarg.h>

// Esta función recibe un especificador de formato (por ejemplo, 'd', 's', 'x', etc.)
// y la lista de argumentos. Llama a la función auxiliar correspondiente
// y retorna la cantidad de caracteres impresos.
int	handle_format(char specifier, va_list args)
{
    int	count = 0;

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

/*
Explicación paso a paso:
- Según el especificador recibido, selecciona el tipo de argumento correcto usando va_arg.
- Llama a la función auxiliar adecuada para imprimir el valor.
- Por ejemplo, para 'd' o 'i' toma un int y llama a ft_putnbr.
- Para 's' toma un char* y llama a ft_putstr.
- Para 'x' o 'X' toma un unsigned int y lo imprime en hexadecimal.
- Para '%' imprime el carácter '%' literal.
- Retorna la cantidad de caracteres impresos por la función