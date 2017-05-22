#include <stddef.h>

char *strncpy(char *dest, const char *src, size_t n)
{
	for (char *tmp = dest; n > 0 && *src != '\0'; --n, ++src, ++tmp)
		*tmp = *src;

	return dest;
}
