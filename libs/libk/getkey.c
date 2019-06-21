#include <kstd.h>

int getkey(void)
{
	int last_pressed = -EAGAIN;

	struct key_event ev;
	while (!readkey(&ev)) {
		if (ev.state == KEY_PRESSED)
			last_pressed = ev.key;
	}

	return last_pressed;
}
