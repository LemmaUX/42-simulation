#include "ft_printf.h"
#include <stdarg.h>
#include <unistd.h>

int	ft_printf(const char *format, ...)
{
    va_list args;
    int		i = 0;
    int		count = 0;

    va_start(args, format);
    while (format[i])
    {
        if (format[i] == '%' && format[i + 1])
        {
            count += handle_format(format[++i], args);
        }
        else
        {
            count += write(1, &format[i], 1);
        }
        i++;
    }
    va_end(args);
    return (count);
}