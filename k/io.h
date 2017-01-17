#ifndef IO_H_
#define IO_H_

#include <k/types.h>

static inline void outb(u16 port, u8 val)
{
	(void)port;
	(void)val;

	// FIXME
}

static inline u8 inb(u16 port)
{
	(void)port;

	// FIXME

	return 0;
}

#endif				/* !IO_H_ */
