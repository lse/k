#include <string.h>
#include <stddef.h>

char *strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; ++i)
		dest[i] = src[i];
	memset(dest + i, '\0', n - i);

	return dest;
}
