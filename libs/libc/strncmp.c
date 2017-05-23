#include <string.h>

int strncmp(const char *s1, const char *s2, size_t n)
{
	for (; *s1 == *s2 && *s1 != '\0' && n > 0; s1++, s2++, n--)
		continue;

	return n ? *s1 - *s2 : 0;
}
