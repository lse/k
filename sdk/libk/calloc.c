#include <stdlib.h>
#include <string.h>

void *calloc(size_t nmemb, size_t size)
{
	void *p = NULL;

	p = malloc(size * nmemb);

	if (p != NULL)
		memset(p, 0, nmemb * size);

	return (p);
}
