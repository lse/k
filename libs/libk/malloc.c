#include <kstd.h>
#include <string.h>

//#define DEBUG_MALLOC

#ifdef DEBUG_MALLOC
#include <stdio.h>
#define pr_debug(fmt, ...) \
	printf("[-] malloc: " fmt, ## __VA_ARGS__)
#else
#define pr_debug(fmt, ...) /* nothing */
#endif

void *malloc(size_t size)
{
	void *ptr = sbrk(size);

	pr_debug("malloc(%u) = %p\n", size, ptr);

	return ptr;
}

void free(void *ptr)
{
	(void)ptr;

	pr_debug("free(%p)\n", ptr);
}
