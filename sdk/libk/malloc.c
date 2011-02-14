#include <kstd.h>
#include <string.h>

void *malloc(size_t size)
{
	return sbrk(size);
}

void free(void *ptr)
{
	(void) ptr;
}

void *realloc(void *ptr, size_t size)
{
	void *res = malloc(size);
	memcpy(res, ptr, size);
	return res;
}
