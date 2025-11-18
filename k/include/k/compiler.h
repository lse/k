#ifndef COMPILER_H
#define COMPILER_H

#define array_size(arr)	(sizeof(arr) / sizeof(*arr))
#define __packed __attribute__((__packed__))
#define align_up(addr, bytes) (((addr) + (bytes) - 1) & ~((bytes) - 1))

#if __has_attribute(__nonstring__)
# define __nonstring                    __attribute__((__nonstring__))
#else
# define __nonstring
#endif

#endif
