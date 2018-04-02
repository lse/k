#ifndef KSTD_H
#define KSTD_H

#include <k/kstd.h>

int write(const void *s, size_t length);
void *sbrk(ssize_t increment);
int getkey(void);
unsigned long gettick(void);
int open(const char *pathname, int flags);
ssize_t read(int fd, void *buf, size_t count);
off_t lseek(int filedes, off_t offset, int whence);
int close(int fd);
int setvideo(int mode);
void swap_frontbuffer(const void *buffer);
int playsound(struct melody *melody, int repeat);
int getmouse(int *x, int *y, int *buttons);
int getkeymode(int mode);

#endif
