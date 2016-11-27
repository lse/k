#include <stdlib.h>
#include <string.h>

void *realloc(void *ptr, size_t size)
{
	void *res = malloc(size);
	memcpy(res, ptr, size);
	return res;
}
