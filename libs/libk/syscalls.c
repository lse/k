/*
* Copyright (c) LSE
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY LSE AS IS AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL LSE BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <kstd.h>
#include <stddef.h>

static inline u32 syscall0(int syscall_nb)
{
	u32 res;

	asm volatile ("int $0x80" : "=a"(res) : "a"(syscall_nb));

	return res;
}

static inline u32 syscall1(int syscall_nb, u32 ebx)
{
	u32 res;

	asm volatile ("int $0x80" : "=a"(res) : "a"(syscall_nb), "b"(ebx));

	return res;
}

static inline u32 syscall2(int syscall_nb, u32 ebx, u32 ecx)
{
	u32 res;

	asm volatile ("int $0x80" : "=a"(res) : "a"(syscall_nb), "b"(ebx), "c"(ecx));

	return res;
}

static inline u32 syscall3(int syscall_nb, u32 ebx, u32 ecx, u32 edx)
{
	u32 res;

	asm volatile ("int $0x80" : "=a"(res) : "a"(syscall_nb), "b"(ebx), "c"(ecx), "d"(edx));

	return res;
}

int write(const void *s, size_t length)
{
	return ((int)syscall2(SYSCALL_WRITE, (u32)s, length));
}

void *sbrk(ssize_t increment)
{
	return ((void *)syscall1(SYSCALL_SBRK, increment));
}

int getkey(void)
{
	return ((int)syscall0(SYSCALL_GETKEY));
}

unsigned long gettick(void)
{
	return ((unsigned long)syscall0(SYSCALL_GETTICK));
}

int open(const char *pathname, int flags)
{
	return ((int)syscall2(SYSCALL_OPEN, (u32)pathname, flags));
}

ssize_t read(int fd, void *buf, size_t count)
{
	return ((ssize_t)syscall3(SYSCALL_READ, fd, (u32)buf, count));
}

off_t lseek(int filedes, off_t offset, int whence)
{
	return ((off_t)syscall3(SYSCALL_SEEK, filedes, offset, whence));
}

int close(int fd)
{
	return ((int)syscall1(SYSCALL_CLOSE, fd));
}

int playsound(struct melody *melody, int repeat)
{
	return ((int)syscall2(SYSCALL_PLAYSOUND, (u32)melody, repeat));
}

int setvideo(int mode)
{
	return ((int)syscall1(SYSCALL_SETVIDEO, mode));
}

void swap_frontbuffer(const void *buffer)
{
	syscall1(SYSCALL_SWAP_FRONTBUFFER, (u32)buffer);
}

int getmouse(int *x, int *y, int *buttons)
{
	return ((int)syscall3(SYSCALL_GETMOUSE, (u32)x, (u32)y, (u32)buttons));
}


void set_palette(unsigned int *new_palette, size_t size)
{
	syscall2(SYSCALL_SETPALETTE, (u32)new_palette, size);
}
