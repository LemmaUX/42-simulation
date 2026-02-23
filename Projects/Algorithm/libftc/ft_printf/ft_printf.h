#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>  /* va_list, va_start, va_arg, va_end */
# include <unistd.h>  /* write() */

/*
** =========================================================
**  ft_printf.h - Header del proyecto ft_printf (42)
** =========================================================
**
** ESTRUCTURA DEL PROYECTO:
**
**   ft_printf.c          → Función principal. Recorre el string de formato,
**                          detecta '%' y delega a handle_format().
**
**   handle_format.c      → Dispatcher. Lee el especificador (c, s, d, ...)
**                          y llama a la función auxiliar correcta.
**
**   ft_putchar.c         → Imprime 1 carácter              → para '%c'
**   ft_putstr.c          → Imprime una cadena              → para '%s'
**   ft_putnbr.c          → Imprime un entero con signo     → para '%d' y '%i'
**   ft_putnbr_base.c     → Imprime entero en cualquier base → para '%u','%x','%X'
**   ft_putptr.c          → Imprime dirección de puntero    → para '%p'
**
** FLUJO GENERAL:
**   ft_printf("Hola %s, tienes %d años\n", nombre, edad)
**     │
**     ├─ 'H','o','l','a',' ' → write() directo
**     ├─ '%s' → handle_format('s') → ft_putstr(nombre)
**     ├─ ',',' ','t','i','e','n','e','s',' ' → write() directo
**     ├─ '%d' → handle_format('d') → ft_putnbr(edad)
**     └─ '\n' → write() directo
*/

/* Función principal - prototipo requerido por el subject */
int	ft_printf(const char *format, ...);

/* Despachador de especificadores de formato */
int	handle_format(char specifier, va_list args);

/* Funciones auxiliares de escritura */
int	ft_putchar(char c);
int	ft_putstr(const char *s);
int	ft_putnbr(int n);
int	ft_putptr(void *ptr);
int	ft_putptr_hex(unsigned long n);
int	ft_putnbr_base(unsigned int n, const char *base);

#endif
