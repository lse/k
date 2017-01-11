#include <string.h>

int write(const char *s, size_t nb);

int puts(const char *s)
{
	return write(s, strlen(s));
}
