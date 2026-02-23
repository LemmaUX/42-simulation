# Libft

## Objetivo

Recrear un subconjunto de funciones estándar de la libc, siguiendo especificaciones estrictas en cuanto a comportamiento, errores y eficiencia.

## Normas

- Ninguna función externa permitida excepto `write`, `malloc`, `free`.
- Todo debe pasar `norminette` (estándar de estilo de 42).
- Compilar con: `gcc -Wall -Wextra -Werror`
- Debe generarse un archivo libft.a

## Parte 1 – Funciones básicas

Implementar las siguientes funciones:

- ft_strlen
- ft_strcpy
- ft_strdup
- ft_memcpy
- ft_memset
- ft_bzero
- ft_isalpha, ft_isdigit, ft_toupper, ft_tolower
- ft_atoi
- ft_calloc

## Parte 2 – Funciones avanzadas

- ft_substr
- ft_strjoin
- ft_split
- ft_itoa
- ft_strmapi

## Output esperado

- Archivo: libft.a
- Header: libft.h
- Makefile funcional con targets: all, clean, fclean, re

## Evaluación

- Funcionalidad completa
- Estilo norminette
- Uso correcto de punteros y memoria
- Sin leaks (valgrind)
- Comportamiento exacto como la versión original

