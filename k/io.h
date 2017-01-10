#ifndef IO_H_
#define IO_H_

#include <k/types.h>

static inline void outb(u16 port, u8 val)
{
	asm volatile ("outb %0, %1" : /* No output */ : "a"(val), "d"(port));
}

static inline u8 inb(u16 port)
{
	u8 res;

	asm volatile ("inb %1, %0" : "=&a"(res) : "d"(port));

	return res;
}

#endif				/* !IO_H_ */
