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
#include <graphic.h>
#include <sound.h>
#include <stdlib.h>

static void splash_screen(void)
{
	struct image *yaka = load_image(RES_PATH "/res/yaka.bmp");
	int blink = 0;
	unsigned long t;

	while (getkey() < 0) {
		t = gettick();
		draw_begin();
		draw_text("YAKANOID", 160 - 4 * 8, 10, 23, 0);
		//draw_image (yaka, 105, 30); // For YAKA2008
		draw_image(yaka, 60, 20);	// For YAKA2009
		if (blink > 5)
			draw_text("Any key to start", 160 - 8 * 8, 170, 184, 0);
		draw_text("Kernel option - LSE - 2007-2008", 5, 190, 208, 0);
		draw_end();
		blink = (blink + 1) % 10;
		while (gettick() - t < 66) ;
	}
}

static void game_loop(void)
{
	struct image *brick_blue = load_image(RES_PATH "/res/blue.bmp");
	struct image *brick_red = load_image(RES_PATH "/res/red.bmp");
	struct image *brick_green = load_image(RES_PATH "/res/green.bmp");
	struct image *ball = load_image(RES_PATH "/res/ball.bmp");
	struct image *bar = load_image(RES_PATH "/res/bar.bmp");
	struct image *current = brick_blue;
	int i, line, pos, k;
	int mtrx[5][10];
	int x, y, dx, dy;
	int bx, bx2, by;
	int score = 0;
	int lives = 3;
	unsigned long t;
	char buf[12] = { 0 };

	for (line = 0; line < 5; line++)
		for (i = 0; i < 10; i++)
			mtrx[line][i] = 1;
	pos = 105;
	x = 105;
	y = 180;
	dx = 1;
	dy = -1;
	while (1) {
		t = gettick();
		draw_begin();

		draw_line(10, 0, 10, 199, 18);
		draw_line(210, 0, 210, 199, 18);

		for (line = 0; line < 5; line++) {
			if (line == 0 || line == 3)
				current = brick_red;
			if (line == 1 || line == 4)
				current = brick_green;
			if (line == 2)
				current = brick_blue;
			for (i = 0; i < 10; i++) {
				if (mtrx[line][i])
					draw_image(current, 10 + i * 20, line * 12);
			}
		}

		k = getkey();
		if (k > 0) {
			if (k == 77)
				pos += 10;
			if (k == 75)
				pos -= 10;
			if (k == 1)
				return;
			if (pos - 25 < 10)
				pos = 35;
			if (pos + 25 >= 210)
				pos = 185;
		}

		x += dx;
		y += dy;

		if (x + 5 >= 210)
			dx = -dx;
		if (x - 5 < 10)
			dx = -dx;

		//draw_fillrect (x - 5, y - 5, x + 5, y + 5, 21, 21);
		draw_image(ball, x - 5, y - 5);
		//draw_fillrect (pos - 25, 190, pos + 25, 199, 20, 20);
		draw_image(bar, pos - 25, 190);

		draw_text("score:", 240, 40, 20, 0);
		sprintf(buf, "%d", score);
		draw_text(buf, 240, 50, 20, 0);

		draw_text("lives:", 240, 150, 20, 0);
		sprintf(buf, "%d", lives);
		draw_text(buf, 240, 160, 20, 0);

		if (y + 5 < 6 * 12) {
			bx = (x - 5 - 10) / 20;
			bx2 = (x + 5 - 10) / 20;
			by = (y - 12) / 12;
			if (mtrx[by][bx] || mtrx[by][bx2]) {
				dy = -dy;
				if (mtrx[by][bx] && mtrx[by][bx2])
					score += 25;
				else
					score += 10;
				mtrx[by][bx] = mtrx[by][bx2] = 0;
			}
		}
		if (y + 5 >= 190) {
			if (x + 5 >= pos - 25 && x - 5 <= pos + 25)
				dy = -dy;
			else {
				lives--;
				if (!lives) {
					draw_text("GAME OVER", 90, 90, 20, 0);
					draw_end();
					for (i = 0; i < 10; i++) {
						t = gettick();
						while (gettick() - t < 100)
							continue;
					}
					return;
				}
				draw_text("try again !", 90, 90, 20, 0);
				draw_end();
				for (i = 0; i < 10; i++) {
					t = gettick();
					while (gettick() - t < 100)
						continue;
				}
				pos = 105;
				x = 105;
				y = 180;
				dx = 1;
				dy = -1;
			}
		}

		while (gettick() - t < 8)
			continue;
		draw_end();
	}
}

void entry(void)
{
	switch_graphic();

	while (1) {
		splash_screen();
		game_loop();
	}
}
