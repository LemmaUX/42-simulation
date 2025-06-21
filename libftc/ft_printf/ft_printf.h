#ifndef FT_PRINTF_H
#define FT_PRINTF_H

#include <stdarg.h>
#include <unistd.h>

// Prototipo principal
int	ft_printf(const char *format, ...);

// Prototipo para manejar los formatos
int	handle_format(char specifier, va_list args);

// Prototipos de funciones auxiliares
int	ft_putchar(char c);
int	ft_putstr(const char *s);
int	ft_putnbr(int n);
int	ft_putptr(void *ptr);
int	ft_putnbr_base(unsigned int n, const char *base);

#endif