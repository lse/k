#include <stddef.h>

void *memchr(const void *s, int c, size_t n)
{
	const char *data = s;

	while (n--)
		if (*data++ == (char)c)
			return (void *)(data - 1);

	return NULL;
}
