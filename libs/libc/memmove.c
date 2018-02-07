#include <stddef.h>
#include <string.h>

void *memmove(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;

	if (s < d && s + n > d) {
		while (n-- > 0)
			d[n - 1] = s[n - 1];
		return dest;
	}

	return memcpy(dest, src, n);
}
