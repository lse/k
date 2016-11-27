#include <string.h>

void *memcpy(void *dest, const void *src, size_t n)
{
	const char *s = src;
	char *d = dest;

	for (; n > 0; n--, d++, s++)
		*d = *s;

	return (dest);
}
