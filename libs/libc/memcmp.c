#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
	const char *d1 = s1;
	const char *d2 = s2;

	for (size_t i = 0; i < n; ++i)
		if (d1[i] != d2[i])
			return d1[i] - d2[i];

	return 0;
}
