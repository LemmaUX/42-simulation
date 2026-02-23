1. ft_strlen
Devuelve la longitud de una cadena.
// ft_strlen.c
#include "libft.h"

size_t ft_strlen(const char *s) {
    size_t len = 0;
    while (s[len])
        len++;
    return len;
}

2. ft_strcpy
Copia una cadena en otra.

// ft_strcpy.c
#include "libft.h"

char *ft_strcpy(char *dst, const char *src) {
    char *ptr = dst;
    while ((*ptr++ = *src++));
    return dst;
}

3. ft_strdup
Duplica una cadena (reserva memoria).
// ft_strdup.c
#include "libft.h"
#include <stdlib.h>

char *ft_strdup(const char *s) {
    size_t len = ft_strlen(s) + 1;
    char *dup = malloc(len);
    if (!dup) return NULL;
    ft_strcpy(dup, s);
    return dup;
}
4. ft_memcpy
Copia n bytes de memoria.
// ft_memcpy.c
#include "libft.h"

void *ft_memcpy(void *dst, const void *src, size_t n) {
    unsigned char *d = dst;
    const unsigned char *s = src;
    if (!dst && !src) return NULL;
    while (n--)
        *d++ = *s++;
    return dst;
}
5. ft_memset
Rellena un bloque de memoria con un valor.
// ft_memset.c
#include "libft.h"

void *ft_memset(void *b, int c, size_t len) {
    unsigned char *ptr = b;
    while (len--)
        *ptr++ = (unsigned char)c;
    return b;
}
