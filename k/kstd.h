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
#ifndef KSTD_H_
# define KSTD_H_

/*
** types
*/
typedef unsigned char		t_uint8;
typedef signed char		t_sint8;
typedef unsigned short		t_uint16;
typedef signed short		t_sint16;
typedef unsigned int		t_uint32;
typedef signed int		t_sint32;
typedef unsigned long long	t_uint64;
typedef signed long long	t_sint64;

typedef unsigned int size_t;
typedef signed int ssize_t;
typedef signed int off_t;

typedef struct
{
  unsigned long	freq;
  unsigned long	duration;
} t_melody;

typedef unsigned int t_palette;

typedef struct
{
  size_t st_size;
} t_stat;


/*
** constants
*/

/* console */
enum
{
  CONS_ESCAPE	= 255,
  CONS_CLEAR	= 1,
  CONS_COLOR	= 2,
  CONS_SETX	= 3,
  CONS_SETY	= 4,
  CONS_BLACK	= 0,
  CONS_BLUE	= 1,
  CONS_GREEN	= 2,
  CONS_CYAN	= 3,
  CONS_RED	= 4,
  CONS_MAGENTA	= 5,
  CONS_YELLOW	= 6,
  CONS_WHITE	= 7,
  CONS_BLINK	= (1 << 7),
  CONS_LIGHT	= (1 << 3)
} e_cons_codes;

# define CONS_FRONT(Color)	(Color)
# define CONS_BACK(Color)	(Color << 4)

/* keyboard */
enum
{
  KEY_ESC		= 1,
  KEY_F1		= 59,
  KEY_F2		= 60,
  KEY_F3		= 61,
  KEY_F4		= 62,
  KEY_F5		= 63,
  KEY_F6		= 64,
  KEY_F7		= 65,
  KEY_F8		= 66,
  KEY_f9		= 67,
  KEY_F10		= 68,
  KEY_F11		= 87,
  KEY_F12		= 88,
  KEY_1			= 2,
  KEY_2			= 3,
  KEY_3			= 4,
  KEY_4			= 5,
  KEY_5			= 6,
  KEY_6			= 7,
  KEY_7			= 8,
  KEY_8			= 9,
  KEY_9			= 10,
  KEY_0			= 11,
  KEY_TAB		= 15,
  KEY_MAJLOCK		= 58,
  KEY_LSHIFT		= 42,
  KEY_RSHIFT		= 54,
  KEY_ALT		= 56,
  KEY_SPACE		= 57,
  KEY_CTRL		= 29,
  KEY_ENTER		= 28,
  KEY_BACKSPACE		= 14,
  KEY_LEFT		= 75,
  KEY_RIGHT		= 77,
  KEY_UP		= 72,
  KEY_DOWN		= 80,
  KEY_PAUSE		= 69,
  KEY_SYST		= 55,
  KEY_INSER		= 82,
  KEY_SUPPR		= 83,
} e_kdb_codes;

/* mouse */
enum
{
  BUTTON_LEFT	= 1,
  BUTTON_RIGHT	= 2
} e_mouse_codes;

/* misc */
# define O_RDONLY	0
# define SEEK_SET	0
# define SEEK_CUR	1
# define SEEK_END	2
# define VIDEO_GRAPHIC	0
# define VIDEO_TEXT	1


/*
** syscalls
*/

/* write */
# define SYSCALL_WRITE		1

int write(const char* s, size_t length);

/* sbrk */
# define SYSCALL_SBRK		2

void* sbrk(ssize_t increment);

/* getkey */
# define SYSCALL_GETKEY		3

int getkey(void);

/* gettick */
# define SYSCALL_GETTICK	4

unsigned long gettick(void);

/* open */
# define SYSCALL_OPEN		5

int open(const char* pathname, int flags);

/* read */
# define SYSCALL_READ		6

ssize_t read(int fd, void* buf, size_t count);

/* seek */
# define SYSCALL_SEEK		7

off_t seek(int filedes, off_t offset, int whence);

/* close */
# define SYSCALL_CLOSE		8

int close(int fd);

/* setvideo */
# define SYSCALL_SETVIDEO	9

int setvideo(int mode);

/* videoaddr */
# define SYSCALL_VIDEOADDR	10

void* videoaddr(void);

/* getpalette */
# define SYSCALL_GETPALETTE	11

void getpalette(t_palette* palette);

/* setpalette */
# define SYSCALL_SETPALETTE	12

void setpalette(t_palette* palette, size_t nb);

/* playsound */
# define SYSCALL_PLAYSOUND	13

int playsound(t_melody*	melody, int repeat);

/* getmouse */
# define SYSCALL_GETMOUSE	14

int getmouse(int* x, int* y, int* buttons);

/* send */
# define SYSCALL_SEND		15

int send(int port, void* data, size_t len);

/* recv */
# define SYSCALL_RECV		16

int recv(int port, void* data, size_t len);

#endif /* !KSTD_H_ */
