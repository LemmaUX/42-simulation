# ft_printf

## 📘 Descripción

El objetivo de este proyecto es recrear una versión propia de la función `printf` de la biblioteca estándar de C, respetando un conjunto de formatos definidos, sin utilizar ninguna función de formateo preexistente.

El proyecto fortalece el uso de punteros, buffers, funciones variádicas (`<stdarg.h>`), parsing y modularidad en C.

---

## ✅ Funcionalidades obligatorias

Tu `ft_printf` debe soportar los siguientes *conversion specifiers*:

| Especificador | Descripción                            |
|---------------|----------------------------------------|
| `%c`          | Imprime un carácter                    |
| `%s`          | Imprime una cadena de caracteres       |
| `%p`          | Imprime una dirección de puntero (hex) |
| `%d` / `%i`   | Imprime un número decimal (signed int) |
| `%u`          | Imprime un número decimal (unsigned)   |
| `%x` / `%X`   | Imprime un número en base hexadecimal  |
| `%%`          | Imprime el carácter `%`                |

Debe retornar el **número total de caracteres impresos**, como lo hace la función original `printf`.

---

## ⚙️ Normas

- Solo se permite el uso de las siguientes funciones externas:
  - `malloc`, `free`, `write`
- Todo el código debe respetar el estándar de codificación 42 (`norminette`)
- Prohibido el uso de `printf`, `sprintf`, `snprintf`, etc.
- Se debe generar una **biblioteca estática**: `libftprintf.a`
- Compilación: `gcc -Wall -Wextra -Werror`

---

## 🧠 Habilidades desarrolladas

- Parsing y manejo de formatos
- Funciones variádicas (`va_list`, `va_start`, `va_arg`, `va_end`)
- Conversión de tipos y manejo de bases numéricas
- Uso de memoria dinámica, buffers temporales
- Desarrollo modular y estructurado en C

---

## 🧪 Ejemplo de uso

```c
#include "ft_printf.h"

int main(void)
{
    int count = ft_printf("Hola %s, tienes %d años\n", "Mundo", 42);
    ft_printf("Caracteres escritos: %d\n", count);
    return (0);
}
ft_printf/
├── ft_printf.h
├── ft_printf.c
├── handle_format.c
├── ft_putchar.c
├── ft_putstr.c
├── ft_putnbr.c
├── ft_putptr.c
├── ft_putnbr_base.c
├── utils/
├── Makefile
└── README.mdft_printf/
├── ft_printf.h
├── ft_printf.c
├── handle_format.c
├── ft_putchar.c
├── ft_putstr.c
├── ft_putnbr.c
├── ft_putptr.c
├── ft_putnbr_base.c
├── utils/
├── Makefile
└── README.md