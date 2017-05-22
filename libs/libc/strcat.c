#include <string.h>

char *strcat(char *dest, const char *src)
{
	return strcpy(dest + strlen(dest), src);
}
