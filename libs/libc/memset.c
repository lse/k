#include <string.h>

void *memset(void *s, int c, size_t n)
{
	unsigned char *p = NULL;

	for (p = s; n > 0; n--, p++)
		*p = c;

	return (s);
}
