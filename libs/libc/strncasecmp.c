#include <stddef.h>
#include <ctype.h>

int strncasecmp(const char *s1, const char *s2, size_t n)
{
	for (; *s1 && n > 0; s1++, s2++, n--)
		if (tolower(*s1) != tolower(*s2))
			break;

	return n ? tolower(*s1) - tolower(*s2) : 0;
}
