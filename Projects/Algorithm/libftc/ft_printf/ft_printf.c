#include "ft_printf.h"
#include <stdarg.h>
#include <unistd.h>

/*
** ft_printf - Función principal que imita el comportamiento de printf.
**
** ¿Qué son los argumentos variádicos (...)?
**   En C, '...' permite que una función reciba un número variable de argumentos.
**   No sabemos de antemano cuántos ni de qué tipo son. Para acceder a ellos
**   usamos la macro va_list junto con:
**     - va_start(args, format) → inicializa la lista de args después de 'format'
**     - va_arg(args, tipo)     → extrae el siguiente argumento del tipo indicado
**     - va_end(args)           → limpia y cierra la lista (obligatorio)
**
** ¿Cómo funciona el bucle?
**   Recorremos el string de formato carácter a carácter:
**   - Si el carácter NO es '%': lo imprimimos directamente con write().
**   - Si el carácter ES '%' y hay un siguiente carácter: es un especificador
**     de formato. Avanzamos el índice (++i) y llamamos a handle_format()
**     para que imprima el argumento correspondiente.
**
** Casos borde:
**   - Si '%' aparece al final del string sin especificador, el while termina
**     porque format[i+1] sería '\0' (condición: format[i+1] != 0).
**   - '%%' → handle_format recibe '%' y imprime un '%' literal.
**
** Retorna: total de caracteres impresos (igual que el printf real).
*/
int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		count;

	i = 0;
	count = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			/* Saltar el '%' y procesar el especificador */
			count += handle_format(format[++i], args);
		}
		else
		{
			/* Carácter normal: imprimir directamente */
			count += write(1, &format[i], 1);
		}
		i++;
	}
	va_end(args);
	return (count);
}
