#include <string.h>

int strcmp(const char *s1, const char *s2)
{
	for (; *s1 == *s2 && *s1 != '\0'; s1++, s2++)
		continue;

	return *s1 - *s2;
}

