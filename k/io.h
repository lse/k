#ifndef IO_H_
# define IO_H_

# include <stddef.h>

static inline void outb(t_uint16 port, t_uint8 val)
{
    asm volatile("outb %0, %1\n\t"
                 : /* No output */
                 : "a" (val), "d" (port));
}

static inline t_uint8 inb(t_uint16 port)
{
    t_uint8 res;

    asm volatile("inb %1, %0\n\t"
                 : "=&a" (res)
                 : "d" (port));

    return res;
}

#endif /* !IO_H_ */
