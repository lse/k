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

/*
 * the splash screen with yaka face.
 */

static void splash_screen(void)
{
	unsigned long t;
	struct image *img = load_image(RES_PATH "/res/yaka.bmp");
	int blink = 0;
	struct melody *intro = load_sound(RES_PATH "/res/intro.csf");

	playsound(intro, -1);

	if (!img)
		blue_screen("Unable to load yaka.bmp");

	while (getkey() < 0) {
		draw_begin();

		draw_image(img, 5, 10);
		draw_text(" Yaka Launcher   ", 160, 50, RED, 0);
		draw_text("powered by Chiche", 160, 60, ORANGE, 0);
		if (blink >= 4)
			draw_text("Any key to start", 160, 90, WHITE, 0);
		draw_text("Kernel option - LSE - 2007-2008", 5, 190, BLUE, 0);
		draw_end();

		/*
		 * 66 ms frame sync
		 */

		blink = (blink + 1) % 8;
		t = gettick();
		while (gettick() - t < 250) ;
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
	struct image *nuage = load_image(RES_PATH "/res/nuage.bmp");
	struct image *sol = load_image(RES_PATH "/res/sol.bmp");
	int angle = 45;
	int x = 0;
	int y = 0;
	int dx = 0;
	int dy = 0;
	int k;
	struct melody *sound = load_sound(RES_PATH "/res/ball.csf");
	struct image *pic[8];
	int fire = 0;
	int power = 0;
	int fx = 0;
	int fy = 0;
	unsigned int gravity = 45;
	int score = 0;
	char buf[12] = { 0 };

	pic[0] = load_image(RES_PATH "/res/1.bmp");
	pic[1] = load_image(RES_PATH "/res/2.bmp");
	pic[2] = load_image(RES_PATH "/res/3.bmp");
	pic[3] = load_image(RES_PATH "/res/4.bmp");
	pic[4] = load_image(RES_PATH "/res/5.bmp");
	pic[5] = load_image(RES_PATH "/res/6.bmp");
	pic[6] = load_image(RES_PATH "/res/7.bmp");
	pic[7] = load_image(RES_PATH "/res/8.bmp");
	int p;
	for (p = 0; p < 8; p++)
		if (pic[p] == NULL)
			blue_screen("Unable to load gif");

	if (!nuage)
		blue_screen("Unable to load nuage.bmp");
	if (!sol)
		blue_screen("Unable to load sol.bmp");

	while (1) {
		p = p % 16;
		t = gettick();

		draw_begin();

		/*
		 * player actions.
		 */

		k = getkey();
		if (k > 0) {
			if (k == KEY_RIGHT)
				angle += 2;
			if (k == KEY_LEFT)
				angle -= 2;
			if (k == KEY_UP)
				gravity += 1;
			if (k == KEY_DOWN)
				gravity -= 1;
			if (k == KEY_ESC)
				break;
			if ((k == KEY_SPACE) && (fire < 3))
				fire = (fire + 1) % 4;
		}

		if (angle >= 90)
			angle = 90;
		if (angle < 0)
			angle = 0;
		if (power >= 90)
			power = 0;

		/*
		 * ball movement.
		 */

		if (fire == 0) {
			power = 0;
			fx = 0;
			fy = 0;
			x = 20 + angle / 2 - fx - 10;
			y = -angle / 2 + fy + 50;
		}
		if (fire == 1)
			power += 2;
		if (fire == 2) {
			fire++;
			dx = power * (angle);
			dy = power * (90 - angle);
		}
		if (fire == 3) {
			dy -= gravity * 10;
			dx -= gravity;
			if (dx < 0)
				dx = 0;

			y += dy / 150;
			if (y < 0)
				y = 0;
			x += dx / 200;

			if (y <= 10) {
				dx -= gravity * 4;
				if (dx < 0) {
					fire = 0;
					score = x;
				}
				playsound(sound, 1);
				dy = -dy;
			}
		}

		/*
		 * computer player bar.
		 */

		/*
		 * draw players.
		 */
		if ((x - 8 - fx) > 200)
			fx += (x - 8 - fx) - 200;

		while ((190 - y + fy) < 8)
			fy += 1;
		while (((190 - y + fy) > 100) && (fy > -10))
			fy--;

		draw_image(nuage, -(fx % nuage->width),
			   (fy % nuage->height) - nuage->height - 40);
		draw_image(nuage, -(fx % nuage->width) + nuage->width,
			   (fy % nuage->height) - nuage->height - 40);
		draw_image(nuage, -(fx % nuage->width) + nuage->width * 2,
			   (fy % nuage->height) - nuage->height - 40);

		draw_image(nuage, -(fx % nuage->width),
			   (fy % nuage->height) - 40);
		draw_image(nuage, -(fx % nuage->width) + nuage->width,
			   (fy % nuage->height) - 40);
		draw_image(nuage, -(fx % nuage->width) + nuage->width * 2,
			   (fy % nuage->height) - 40);

		if (fy < 100) {
			draw_image(sol, -(fx % nuage->width), fy - 40);
			draw_image(sol, -(fx % nuage->width) + sol->width,
				   fy - 40);
			draw_image(sol, -(fx % nuage->width) + sol->width * 2,
				   fy - 40);
		} else {
			draw_image(nuage, -(fx % nuage->width),
				   (fy % nuage->height) - 40);
			draw_image(nuage, -(fx % nuage->width) + nuage->width,
				   (fy % nuage->height) - 40);
			draw_image(nuage,
				   -(fx % nuage->width) + nuage->width * 2,
				   (fy % nuage->height) - 40);
		}

		//draw_line (0,   199 + fy,    319,     199 + fy,   64);
		//draw_line (0,   198 + fy,    319,     198 + fy,   192);

		draw_rect(0 - fx, 50 + fy, 10 - fx, 145 + fy, 70);
		draw_fillrect(0 - fx, 140 - power + fy, 10 - fx, 145 + fy, 60,
			      70);

		draw_rect(15 + angle / 2 - fx, 145 + angle / 2 + fy,
			  20 + angle / 2 - fx, 150 + angle / 2 + fy, 60);
		draw_line(0 - fx, 200 + fy, 20 + angle / 2 - fx - 2,
			  150 + angle / 2 + fy - 2, 60);

		draw_image_alpha(pic[p / 2], x - 8 - fx, 170 - y + fy, 250);

		draw_text("Gravity: ", 0, 0, 162, -1);
		sprintf(buf, "%d", gravity);
		draw_text(buf, 70, 0, 162, -1);

		if ((fire == 0) && (score != 0)) {
			draw_text("Score: ", 100, 60, 12, -1);
			sprintf(buf, "%d", score);
			draw_text(buf, 170, 60, 12, -1);
		}

		draw_end();

		/*
		 * 33 ms sync between each frame.
		 */

		while (gettick() - t <= 66) ;
		p += 1;
	}

	clear_sound(sound);
	clear_image(nuage);
	clear_image(sol);
	int l = 0;
	for (l = 0; l < 8; l++)
		clear_image(pic[l]);
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
