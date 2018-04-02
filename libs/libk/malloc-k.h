#include <k/kstd.h>
#include <k/types.h>
#include <stdio.h>

#define HAVE_MMAP 0
#define USE_LOCKS 0
#define NO_MALLOC_STATS 1

#define LACKS_TIME_H
#define LACKS_UNISTD_H
#define LACKS_STDLIB_H
#define LACKS_ERRNO_H
#define LACKS_SYS_TYPES_H

#define malloc_getpagesize 4096

#define MALLOC_FAILURE_ACTION	\
	do {	\
		printf("[!] malloc internal error \n"); \
	} while(0)


#define USAGE_ERROR_ACTION(m, p)	\
	do {  \
		printf("[!] malloc: corrupted chunk: 0x%x\n", p); \
	} while (0)

#define abort() \
	do {						\
		printf("[!!!] malloc: abort\n");	\
		for (;;)				\
			continue;			\
	} while(0)
