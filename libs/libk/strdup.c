#include <string.h>
#include <stdlib.h>

char *strdup(const char *s)
{
	char *r = NULL;
	char *p = NULL;

	r = malloc(strlen(s) + 1);
	if (!r)
		return NULL;

	for (p = r; *s != '\0'; s++, p++)
		*p = *s;

	*p = '\0';

	return (r);
}

