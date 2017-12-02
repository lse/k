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

unsigned long jiffies = 0;
char *trick[10];
int hhh[10];

struct {
	enum e_gfx trick;
	char *name;
	int points;
	int duration;
} tricks[] = {
	{TRICK_ROTATE, "360", 30, 25},
	{TRICK_ROTATE, "720", 30, 25},
	{TRICK_FLIP, "flip", 20, 20},
	{TRICK_SLIDE, "slide", 1, 1},
	{TRICK_NONE, NULL, 0, 0}
};

static void draw_trick(void)
{
	int i;
	int j;

	for (i = 0; tricks[i].name; i++)
		if (tricks[i].trick == skater.trick)
			break;

	if (tricks[i].trick != TRICK_NONE && tricks[i].trick != TRICK_SLIDE)
		if (skater.trick_delay == tricks[i].duration - 1) {
			//      if (skater.rotation < 720)
			for (j = 0; j < 9; j++) {
				trick[j] = trick[j + 1];
				hhh[j] = hhh[j + 1];
			}

			trick[9] = tricks[i].name;
			hhh[9] = skater.y - skater.height - 10;
			if (hhh[9] <= hhh[8] + 9)
				hhh[9] = hhh[8] + 10;

		}

	for (i = 0; i < 10; i++) {
		if (--hhh[i] <= 20)
			trick[i] = NULL;

		if (trick[i])
			draw_text(trick[i], skater.x + 10, hhh[i], YELLOW,
				  BG_COLOR);
	}
}

static void splash_screen(void)
{
	unsigned long t;
	int blink = 0;
	struct image *logo = load_image(RES_PATH "/res/logo.bmp");

	while (getkey() < 0) {
		draw_clear(BG_COLOR);

		draw_image(logo, 10, 20);

		draw_text("K skater", 200, 40, RED, BG_COLOR);

		draw_text("Kernel option - LSE - 2006-2007", 5, 190, BLUE,
			  BG_COLOR);
		if (blink >= 5)
			draw_text("Any key to start", 170, 90, WHITE, 0);
		draw_end();

		blink = (blink + 1) % 10;
		t = gettick();
		while (gettick() - t < 66) ;
	}
}

void speed(void)
{
	int x = 10;
	int y = 120;

	if (scrolling.freq < 6)
		draw_fillrect(x, y - 10, x + 7, y, BLACK, GREEN);
	if (scrolling.freq < 5)
		draw_fillrect(x, y - 20, x + 7, y - 10, BLACK, YELLOW);
	if (scrolling.freq < 4)
		draw_fillrect(x, y - 30, x + 7, y - 20, BLACK, ORANGE);
	if (scrolling.freq < 3)
		draw_fillrect(x, y - 40, x + 7, y - 30, BLACK, RED);
	if (scrolling.freq < 2)
		draw_fillrect(x, y - 50, x + 7, y - 40, BLACK, PURPLE);
	if (scrolling.freq < 1)
		draw_fillrect(x, y - 60, x + 7, y - 50, BLACK, AQUA);
}

void draw_score(void)
{
	char buf[12] = { 0 };

	draw_trick();

	//  draw_text("Time: ", 10, 10, PURPLE, BG_COLOR);
	sprintf(buf, "%d", jiffies / 50);
	draw_text(buf, 10, 10, PURPLE, BG_COLOR);

	draw_text("Trick: ", GRAPHIC_WIDTH / 2 - 45, 10, BLUE, BG_COLOR);
	sprintf(buf, "%d", score_tmp);
	draw_text(buf, GRAPHIC_WIDTH / 2 + 5, 10, BLUE, BG_COLOR);
	if (skater.combo > 1) {
		draw_text("Combo x", GRAPHIC_WIDTH / 2 - 45, 20, RED, BG_COLOR);
		sprintf(buf, "%d", skater.combo);
		draw_text(buf, GRAPHIC_WIDTH / 2 + 15, 20, RED, BG_COLOR);
	}

	draw_text("Score: ", GRAPHIC_WIDTH - 90, 10, GREEN, BG_COLOR);
	sprintf(buf, "%d", score);
	draw_text(buf, GRAPHIC_WIDTH - 40, 10, GREEN, BG_COLOR);
}

/*
 * the splash screen with sexy chiche's face.
 */

static void game_loop(void)
{
	while (1) {
		unsigned long t = gettick();
		if (!skater.trick_delay)
			switch (getkey()) {
			case KEY_SPACE:
				skater_jump(scrolling.freq);
				break;
			case KEY_RIGHT:
				scroll_speedup();
				break;
			case KEY_DOWN:
				skater_slide();
				break;
			case KEY_LEFT:
				skater_rotate();
				break;
			case KEY_UP:
				skater_flip();
				break;
			case KEY_ESC:
				return;
			default:
				break;
			}

		draw_clear(BG_COLOR);
		//      background();
		park_draw();
		skater_draw();
		draw_score();
		speed();
		draw_end();

		/*
		 * 8ms between frames
		 */
		while (gettick() - t <= 8) ;
		jiffies++;

		scroll();
	}
}

/*
 * game entry point.
 */

void entry(void)
{
	int i;

	gfx_init();

	switch_graphic();

	while (1) {
		splash_screen();

		for (i = 0; i < 10; i++)
			trick[i] = NULL;

		jiffies = 0;
		score = 0;
		park_init();
		skater_init();

		game_loop();
	}

	/*
	 * we should never arrive here...
	 */
	switch_text();
}
