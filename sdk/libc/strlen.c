#include <string.h>
#include <stddef.h>

size_t strlen(const char *s)
{
	const char *p = NULL;

	for (p = s; *p != '\0'; p++)
		continue;

	return (p - s);
}
