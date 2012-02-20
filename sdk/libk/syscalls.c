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

// int80.S
int int80(int syscall_nb, ...);

int write(const char* s, size_t length)
{
  return ((int) int80(SYSCALL_WRITE, s, length));
}

void* sbrk(ssize_t increment)
{
  return ((void*) int80(SYSCALL_SBRK, increment));
}

int getkey(void)
{
  return ((int) int80(SYSCALL_GETKEY));
}

unsigned long gettick(void)
{
  return ((unsigned long) int80(SYSCALL_GETTICK));
}

int open(const char* pathname, int flags)
{
  return ((int) int80(SYSCALL_OPEN, pathname, flags));
}

ssize_t	read(int fd, void* buf, size_t count)
{
  return ((ssize_t) int80(SYSCALL_READ, fd, buf, count));
}

off_t seek(int filedes, off_t offset, int whence)
{
  return ((off_t) int80(SYSCALL_SEEK, filedes, offset, whence));
}

int close(int fd)
{
  return ((int) int80(SYSCALL_CLOSE, fd));
}


int playsound(t_melody* melody, int repeat)
{
  return ((int) int80(SYSCALL_PLAYSOUND, melody, repeat));
}

int setvideo(int mode)
{
  return ((int) int80(SYSCALL_SETVIDEO, mode));
}

void swap_frontbuffer(const void *buffer)
{
  return int80(SYSCALL_SWAP_FRONTBUFFER, buffer);
}

void getpalette(t_palette* palette)
{
  int80(SYSCALL_GETPALETTE, palette);
}

void setpalette(t_palette* palette, size_t nb)
{
  int80(SYSCALL_SETPALETTE, palette, nb);
}

int getmouse(int* x, int* y, int* buttons)
{
  return ((int) int80(SYSCALL_GETMOUSE, x, y, buttons));
}

int send(int port, void* data, size_t len)
{
  return ((int) int80(SYSCALL_SEND, port, data, len));
}

int recv(int port, void* data, size_t len)
{
  return ((int) int80(SYSCALL_RECV, port, data, len));
}

int getkeymode(int mode)
{
  return ((int) int80(SYSCALL_GETKEYMODE, mode));
}
