#include <string.h>

size_t strnlen(const char *s, size_t maxlen)
{
	size_t i = 0;
	for (; i < maxlen; ++i)
		if (!s[i])
			return i;
	return i;
}
