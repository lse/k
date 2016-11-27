#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
	const char *d1 = s1;
	const char *d2 = s2;

	for (; *d1 == *d2 && n > 0; d1++, d2++, n--)
		continue;

	return (n);
}
