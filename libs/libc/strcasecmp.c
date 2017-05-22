#include <ctype.h>

int strcasecmp(const char *s1, const char *s2)
{
	for (; *s1; s1++, s2++)
		if (tolower(*s1) != tolower(*s2))
			break;

	return tolower(*s1) - tolower(*s2);
}
