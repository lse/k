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
#include "skate.h"

#define array_size(A) (sizeof (A) / sizeof (*A))

static const enum e_gfx park_begin[] = {
	NO_BOX, NO_BOX, NO_BOX, NO_BOX, BOX_UP, BOX, BOX, NO_BOX, NO_BOX, BOX_UP
};

static const enum e_gfx park_loop[] = {
	NO_BOX, BOX_RAIL, BOX_RAIL, NO_BOX, BOX_UP, BOX, BOX, NO_BOX, NO_BOX, BOX_UP
};

struct scrolling scrolling;

enum e_gfx get_park(size_t box) {
	if (box < array_size(park_begin))
		return park_begin[box];

	return park_loop[(box - array_size(park_begin)) % array_size(park_loop)];
}

void scroll_stop(void)
{
	scrolling.timer1 = scrolling.timer2 = jiffies;
	scrolling.freq = 6;
}

static void scroll_init(void)
{
	scrolling.step = 2;
	scrolling.width = 0;
	scrolling.current_box = 0;
	scrolling.x = 0;
	scroll_stop();
}

void park_init(void)
{
	scroll_init();
}

int get_box(int x)
{
	return x / BOX_WIDTH;
}

int get_shift(int x)
{
	return x % BOX_WIDTH;
}

int box_height(int box, int shift)
{
	switch (get_park(box)) {
	case BOX_UP:
		return shift;
	case BOX_RAIL:
		if (skater.slide)
			return 15;
		else
			return 0;
	case BOX:
		return BOX_HEIGHT;
	default:
		return 0;
	}
}

void park_draw(void)
{
	int box = get_box(scrolling.x);
	int x = -get_shift(scrolling.x);

	do {
		enum e_gfx p = get_park(box);
		if (graphics[p].gfx)
			draw_image_alpha(graphics[p].gfx,
					 x, GRAPHIC_HEIGHT - 10 - BOX_HEIGHT,
					 BG_COLOR);

		x += BOX_WIDTH;
		box++;
	} while (x < GRAPHIC_WIDTH);
}

void scroll(void)
{
	// automatic slow down
	if (!skater.slide)
		if (jiffies - scrolling.timer1 >= 20) {
			scrolling.timer1 = jiffies;
			scroll_slowdown();
		}
	// scroll left
	if (jiffies - scrolling.timer2 >= (unsigned long)scrolling.freq) {
		scrolling.timer2 = jiffies;
		if (scrolling.freq < 6) {
			if (skater.x < GRAPHIC_WIDTH / 2 - BOX_WIDTH)
				skater.x += scrolling.step;
			else {
				scrolling.x += scrolling.step;
				scrolling.width += scrolling.step;
			}
		}
	}
	//
	if (scrolling.width == BOX_WIDTH) {
		scrolling.width = 0;
		scrolling.current_box++;
	}
}

int scroll_slowdown(void)
{
	scrolling.freq++;
	if (scrolling.freq > 6)
		scrolling.freq = 6;
	return scrolling.freq;
}

int scroll_speedup(void)
{
	scrolling.freq -= 2;
	if (scrolling.freq <= 0)
		scrolling.freq = 0;
	return scrolling.freq;
}
