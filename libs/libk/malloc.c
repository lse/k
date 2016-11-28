#include <kstd.h>
#include <string.h>

void *malloc(size_t size)
{
	return sbrk(size);
}

void free(void *ptr)
{
	(void)ptr;
}
