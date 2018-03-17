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

/*
 * the splash screen with sexy chiche's face.
 */

static void splash_screen(void)
{
	unsigned long t;
	struct image *img = load_image(RES_PATH "/res/chiche.bmp");
	int blink = 0;
	struct melody *intro = load_sound(RES_PATH "/res/intro.ksf");

	playsound(intro, -1);

	if (!img)
		blue_screen("Unable to load chiche.bmp");

	while (getkey() < 0) {
		t = gettick();

		draw_begin();
		draw_image(img, 5, 10);
		draw_text("   Chiche Pong   ", 160, 50, RED, 0);
		draw_text("powered by Chiche", 160, 60, ORANGE, 0);
		if (blink >= 5)
			draw_text("Any key to start", 160, 90, WHITE, 0);
		draw_text("Kernel option - LSE - 2006-2007", 5, 190, BLUE, 0);
		draw_end();

		/*
		 * 66 ms frame sync
		 */

		blink = (blink + 1) % 10;
		while (gettick() - t <= 66) ;
	}

	playsound(NULL, -1);

	clear_sound(intro);
	clear_image(img);
}

/*
 * game loop.
 */

static void game_loop(void)
{
	unsigned long t;
	struct image *ball = load_image(RES_PATH "/res/ball.bmp");
	int player1;
	int player2;
	int x, y;
	int dx, dy;
	int k;
	int standby = 80;
	struct melody *sound = load_sound(RES_PATH "/res/ball.ksf");

	if (!ball)
		blue_screen("Unable to load ball.bmp");

	player1 = 100;
	x = 160;
	y = 100;
	dx = 2;
	dy = 2;

	while (1) {
		t = gettick();

		draw_begin();

		/*
		 * draw screen borders
		 */

		draw_line(0, 0, 319, 0, 64);
		draw_line(0, 199, 319, 199, 64);
		draw_line(0, 1, 319, 1, 192);
		draw_line(0, 198, 319, 198, 192);

		if (!standby) {
			/*
			 * player actions.
			 */

			k = getkey();
			if (k > 0) {
				if (k == KEY_DOWN)
					player1 += 5;
				if (k == KEY_UP)
					player1 -= 5;
				if (k == KEY_ESC)
					break;
			}
		}
		if (player1 + 20 >= 200)
			player1 = 179;
		if (player1 - 20 < 0)
			player1 = 20;

		/*
		 * ball movement.
		 */

		if (!standby) {
			x += dx;
			y += dy;
		}

		/*
		 * edges.
		 */

		if (y + 8 >= 200) {
			y = 191;
			dy = -dy;
			playsound(sound, 1);
		}
		if (y - 8 < 0) {
			y = 8;
			dy = -dy;
			playsound(sound, 1);
		}

		/*
		 * computer player bar.
		 */

		player2 = y;
		if (player2 + 20 >= 200)
			player2 = 179;
		if (player2 - 20 < 0)
			player2 = 20;

		if (standby) {
			draw_text("Ready ?", 130, 96, RED, 0);
			standby--;
		}

		/*
		 * draw players.
		 */

		draw_fillrect(0, player1 - 20, 10, player1 + 20, 71, 71);
		draw_fillrect(309, player2 - 20, 319, player2 + 20, 23, 23);

		if (!standby)
			draw_image(ball, x - 8, y - 8);

		if (x + 5 >= 309) {
			dx = -dx;
			playsound(sound, 1);
		}
		if (x - 5 < 10) {
			/*
			 * player bar collision test.
			 */

			if (player1 - 20 < y + 3 && player1 + 20 > y - 3) {
				dx = -dx;
				playsound(sound, 1);
			} else {
				draw_text("You loose...", 112, 96, RED, 0);
				draw_end();
				while (gettick() - t < 1000) ;
				player1 = 100;
				x = 160;
				y = 100;
				dx = 3;
				dy = 3;
				standby = 80;
				continue;
			}
		}

		draw_end();

		/*
		 * 33 ms sync between each frame.
		 */

		while (gettick() - t <= 33) ;
	}

	clear_sound(sound);
	clear_image(ball);
}

/*
 * game entry point.
 */

void entry(void)
{
#if 0
	unsigned long t;

	printf
	    ("\n\n\n\t== La demonstration qui suit a ete realisee sans trucages ==\n\n\n\n\n");

	t = gettick();
	while (gettick() - t < 7000) ;
#endif

	switch_graphic();

	while (1) {
		splash_screen();
		game_loop();
	}

	/*
	 * we should never arrive here...
	 */

	switch_text();
}
