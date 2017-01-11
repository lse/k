#include <string.h>
#include <stddef.h>

size_t strlen(const char *s)
{
	const char *p = s;

	while (*p)
		p++;

	return (p - s);
}
