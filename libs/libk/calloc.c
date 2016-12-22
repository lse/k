#include <stdlib.h>
#include <string.h>

void *calloc(size_t nmemb, size_t size)
{
	void *p = malloc(size * nmemb);

	if (!p)
		return NULL;

	memset(p, 0, nmemb * size);

	return (p);
}
