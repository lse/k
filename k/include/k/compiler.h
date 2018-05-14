#ifndef COMPILER_H
#define COMPILER_H

#define array_size(arr)	(sizeof(arr) / sizeof(*arr))
#define __packed __attribute__((__packed__))
#define align_up(addr, bytes) (((addr) + (bytes) - 1) & ~((bytes) - 1))

#endif
