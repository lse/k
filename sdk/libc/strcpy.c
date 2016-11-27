#include <string.h>

char *strcpy(char *dest, const char *src)
{
	char *p = NULL;

	for (p = dest; *src != '\0'; p++, src++)
		*p = *src;

	*p = '\0';

	return (dest);
}
