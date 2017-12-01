#include <k/kstd.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void panic(const char *fmt, ...)
{

	puts("[!] PANIC: ");

	va_list args;
	va_start(args, fmt);
	char buf[256];

	vsprintf(buf, fmt, args);
	va_end(args);

	puts(buf);
	puts("\n");

	for (;;)
		asm volatile("hlt");
}

__attribute__ ((weak))
size_t write(const void *buffer, size_t len)
{
	(void) buffer;
	(void) len;

	static const char panic_buf[] = {
		'P',
		CONS_RED | CONS_BACK(CONS_BLACK),
		'A',
		CONS_RED | CONS_BACK(CONS_BLACK),
		'N',
		CONS_RED | CONS_BACK(CONS_BLACK),
		'I',
		CONS_RED | CONS_BACK(CONS_BLACK),
		'C',
		CONS_RED | CONS_BACK(CONS_BLACK),
	};

	memcpy((void *)0xb8000, panic_buf, sizeof (panic_buf));

	for (;;)
		asm volatile ("hlt");
}
