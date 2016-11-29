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
#include <stdio.h>

#include <k/kstd.h>

/*
 * weak definition of write(), will be replaced by the right one at link time
 */
__attribute__((weak)) int write(const char *buf, size_t len)
{
	(void)buf;
	(void)len;

	asm ("ud2");

	return -1;
}

/*
 * some constants and define
 */

static const char digits[] = "0123456789abcdef";

#define INT_UNSIGNED	1
#define INT_SIGNED	2

#define BASE_HEX	16
#define BASE_OCT	8

#define MY_BUF_SIZE	32

/*
 * some types
 */

struct s_file {
	char buff[MY_BUF_SIZE];
	size_t pos;
};

union u_arg {
	unsigned long value;
	unsigned int uint;
	int sint;
	void *ptr;
};

typedef int (*t_fmtfun) (union u_arg * arg, struct s_file * file, int flags);

struct s_format {
	char ch;
	t_fmtfun fun;
	int flags;
};

/*
 * buffered I/O
 */

static void my_fflush(struct s_file *file)
{
	write(file->buff, file->pos);
	file->pos = 0;
}

static struct s_file *init_buffered_output(void)
{
	static struct s_file res;

	res.pos = 0;
	return &res;
}

static void my_putc(char c, struct s_file *file)
{
	file->buff[file->pos++] = c;
	if (file->pos == MY_BUF_SIZE)
		my_fflush(file);
}

static void close_buffered_output(struct s_file *file)
{
	my_fflush(file);
}

/*
 * formatting functions
 */

static int conv(unsigned int n, int base, int dig[])
{
	int i = 0;

	while (n) {
		dig[i] = n % base;
		++i;
		n /= base;
	}
	return i - 1;
}

static int my_printnbr_base(unsigned int n,
			    const char base[], int card, struct s_file *file)
{
	int digits[96];
	int i;
	int count;

	if (n == 0) {
		my_putc('0', file);
		return 1;
	}
	count = i = conv(n, card, digits);
	for (; i >= 0; --i)
		my_putc(base[digits[i]], file);
	return count;
}

static int print_int(union u_arg *value, struct s_file *file, int flags)
{
	int sh = 0;

	if (value->sint == 0) {
		my_putc('0', file);
		return 1;
	}
	if (flags == INT_SIGNED) {
		if (value->sint < 0) {
			my_putc('-', file);
			value->uint = -value->sint;
			sh = 1;
		} else
			value->uint = value->sint;
	}
	return my_printnbr_base(value->uint, digits, 10, file) + sh;
}

static int print_str(union u_arg *value, struct s_file *file, int flags)
{
	int count = 0;
	char *s = value->ptr;

	flags = flags;
	for (; *s; ++count, ++s) {
		if (*s - CONS_COLOR == 0) {
			my_putc(*s, file);
			s++;
			++count;
		}

		my_putc(*s, file);
	}
	return count;
}

static int print_char(union u_arg *value, struct s_file *file, int flags)
{
	char c;

	flags = flags;
	c = value->sint;
	my_putc(c, file);
	return 1;
}

static int print_base(union u_arg *value, struct s_file *file, int flags)
{
	return my_printnbr_base(value->uint, digits, flags, file);
}

static int print_ptr(union u_arg *value, struct s_file *file, int flags)
{
	int s = 0;
	union u_arg val;

	flags = flags;

	val.ptr = "0x";
	s += print_str(&val, file, 0);
	return s + my_printnbr_base(value->uint, digits, BASE_HEX, file);
}

static const struct s_format formats[] = {
	{'d', print_int, INT_SIGNED},
	{'i', print_int, INT_SIGNED},
	{'u', print_int, INT_UNSIGNED},
	{'s', print_str, 0},
	{'c', print_char, 0},
	{'o', print_base, BASE_OCT},
	{'x', print_base, BASE_HEX},
	{'p', print_ptr, 0},
	{0, NULL, 0}
};

static int special_char(char fmt, union u_arg *value, struct s_file *file)
{
	int i;

	for (i = 0; formats[i].fun; ++i)
		if (formats[i].ch == fmt)
			break;
	if (formats[i].fun)
		return formats[i].fun(value, file, formats[i].flags);
	else {
		if (fmt != '%')
			my_putc('%', file);
		my_putc(fmt, file);
		return 1 + (fmt != '%');
	}
}

/*
 * finally, printf
 */

int vprintf(const char *format, va_list args)
{
	struct s_file *file;
	union u_arg arg;
	int count = 0;

	file = init_buffered_output();
	for (; *format; format += (*format == '%' ? 2 : 1)) {
		if (*format == '%') {
			arg.value = va_arg(args, unsigned long);
			count += special_char(*(format + 1), &arg, file);
		} else {
			if (*format - CONS_COLOR == 0) {
				my_putc(*format, file);
				format++;
				++count;
			}

			my_putc(*format, file);
			++count;
		}
	}
	close_buffered_output(file);
	return count;
}

int printf(const char *format, ...)
{
	int res;
	va_list args;

	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	return res;
}
