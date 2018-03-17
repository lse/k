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

static int splash_screen(void)
{
	unsigned long t;
	struct image *imgchiche = load_image(RES_PATH "/res/chiche_b.bmp");
	struct image *imgchef = load_image(RES_PATH "/res/chef_big.bmp");
	int blink = 0;
	struct melody *intro = load_sound(RES_PATH "/res/intro.csf");
	int sel = 1;
	int k = 0;
	int mouse_x, mouse_y, buttons;

	playsound(intro, -1);

	if (!imgchef || !imgchiche)
		blue_screen("Unable to load chiche.bmp");

	getmouse(&mouse_x, &mouse_y, &buttons);
	while (k != KEY_ENTER && !(buttons & 1)) {
		t = gettick();

		getmouse(&mouse_x, &mouse_y, &buttons);
		k = getkey();
		if (k == KEY_UP || mouse_y > 0)
			sel = 1;

		if (k == KEY_DOWN || mouse_y < 0)
			sel = 0;

		draw_begin();
		draw_image(imgchiche, 5, 10);
		draw_image(imgchef, 5, 100);
		draw_text("   ChicheHunter   ", 160, 50, RED, 0);
		if (blink >= 5)
			draw_text("Any key to start", 160, 90, WHITE, 0);
		draw_text("Monde de Merde 2008", 5, 190, BLUE, 0);

		if (sel)
			draw_text("<==", 80, 40, BLUE, 0);
		else
			draw_text("<==", 80, 130, BLUE, 0);

		draw_end();

		/*
		 * 66 ms frame sync
		 */

		blink = (blink + 1) % 10;
		while (gettick() - t < 33)
			continue;
	}

	playsound(NULL, -1);

	clear_sound(intro);
	clear_image(imgchef);
	clear_image(imgchiche);
	return sel;
}

#define WIDTH 320

#define MONSTERS_COLS 6
#define NB_MONSTERS 20
#define MONSTER_X_BBOX 20
#define MONSTER_Y_BBOX 24

#define P_SIZE 15
#define NUM_MOVE 20
#define GAP_HIT 7

#define FACTOR_X 1.7
#define FACTOR_Y 1

/*
 * game loop.
 */

static void game_loop(struct image * img)
{
	unsigned long t;
	int px, py;
	int k;
	float mx = 0, my = 0, mdx, mdy, max, may, alive;
	float les_mx[] = {
		320, 0, 0, 0, 320, 0, 320, 320, 320, 0, 0, 320, 0, 320, 320, 0, 0, 320, 320, 320, 0
	};
	float les_dx[] = {
		-5, 5, 5, 5, -5, 5, -5, -5, -5, 5, 5, -5, 5, -5, -5, 5, 5, -5, -5, -5, 5
	};
	float les_my[] = {
		50, 30, 20, 35, 50, 25, 25, 20, 50, 40, 46, 41, 36, 30, 28, 47, 33, 42, 51, 46
	};
	float les_dy[] = {
		-2, -2, -1, -3, -2, -1, -2, -2, -2.5, -1, -3, -2, -1, -1, -3, -1, -2, -1, -2, -2
	};
	int les_index = 0;
	int i = 0;
	int standby = 0;
	int fin = 0;
	int monsters = -1;
	int hit[NB_MONSTERS];
	int dead_monsters = 0;
	struct melody *sound = load_sound(RES_PATH "/res/ball.csf");
	int mouse_x, mouse_y, buttons;
	struct image *bush = load_image(RES_PATH "/res/bush.bmp");
	char score[] = "score: xx/xx";
	int choot, can_choot;

	if (!img)
		blue_screen("Unable to load chiche.bmp");

	px = 160;
	py = 100;
	mdx = 0;
	mdy = 0;
	max = 0;
	may = 0.1;
	alive = 0;
	can_choot = 1;

	for (i = 0; i < NB_MONSTERS; i++)
		hit[i] = 0;

	while (1) {
		t = gettick();

		choot = 0;

		draw_begin();
		draw_clear(240);

		getmouse(&mouse_x, &mouse_y, &buttons);
		k = getkey();
		if (k > 0) {
			if (k == KEY_ESC)
				standby = !standby;

			if ((standby || fin) && k == KEY_ENTER)
				return;
		}
		if (!standby && !fin) {
			px += mouse_x;
			py -= mouse_y;

			if (can_choot && (buttons & 1)) {
				choot = 1;
				can_choot = 0;
			}

			if (!(buttons & 1))
				can_choot = 1;

			if (px <= 10)
				px = 11;
			if (px >= 320 - 10)
				px = 320 - 11;
			if (py <= 10)
				py = 11;
			if (py >= 200 - 10)
				py = 200 - 11;
		}

		/*
		 * move and draw monsters.
		 */
		if (alive) {
			if (!standby && !fin) {
				mdx += max;
				mdy += may;
				my += mdy;
				mx += mdx;
			}

			if (my > 150 || mx < 0 || mx > 320) {
				alive = 0;
				hit[monsters] = 0;
			}

			if (!fin)
				draw_image_alpha(img, mx, my, 0);
		} else {
			alive = 1;
			monsters++;
			mx = les_mx[les_index];
			my = les_my[les_index];
			mdy = les_dy[les_index] * FACTOR_Y;
			mdx = les_dx[les_index] * FACTOR_X;
			les_index = (les_index + 1) % NUM_MOVE;
			if (monsters == NB_MONSTERS)
				fin = 1;
		}

		/*
		 * draw BUSH.
		 */
		draw_fillrect(0, 150, 320, 200, 0, 0);
		draw_image_alpha(bush, 0, 0, 0);

		/*
		 * draw player.
		 */
		//draw_image (img, px, py);
		draw_line(px, py - 10, px, py + 10, RED);
		draw_line(px - 10, py, px + 10, py, RED);

		/*
		 * draw, move and inpact bullet.
		 */
		if (!standby && !fin && (buttons & 1)) {
			for (i = 0; i < NB_MONSTERS; i++) {
				if (alive && choot
				    && (px >= mx && px <= (mx + MONSTER_X_BBOX))
				    && (py >= my
					&& py <= (my + MONSTER_Y_BBOX))) {
					playsound(sound, 1);
					dead_monsters++;
					alive = 0;
					hit[monsters] = 1;
					break;
				}
			}
		}

		/*
		 * draw HIT.
		 */
		for (i = 0; i < NB_MONSTERS; i++)
			draw_text("|", 79 + i * GAP_HIT, 182,
				  hit[i] ? WHITE : RED, BLACK);

		/*
		 * draw PAUSE.
		 */
		if (standby && !fin) {
			draw_text("PAUSE", 135, 50, WHITE, BLACK);
			draw_text("ESC key to continue", 90, 70, WHITE, BLACK);
			draw_text("ENTER key to quit", 93, 90, WHITE, BLACK);
		}

		/*
		 * draw FIN.
		 */
		if (fin) {
			score[7] = dead_monsters / 10 + '0';
			score[8] = dead_monsters % 10 + '0';
			score[10] = NB_MONSTERS / 10 + '0';
			score[11] = NB_MONSTERS % 10 + '0';
			draw_text(score, 110, 70, WHITE, BLACK);
			draw_text("ENTER key to quit", 90, 100, WHITE, BLACK);
		}

		draw_end();

		/*
		 * 33 ms sync between each frame.
		 */
		while (gettick() - t <= 10)
			continue;
	}

	clear_image(img);
}

/*
 * game entry point.
 */

void entry(void)
{
	switch_graphic();

	while (1) {
		game_loop(splash_screen()? load_image(RES_PATH "/res/chiche.bmp") :
			  load_image(RES_PATH "/res/chef.bmp"));
	}
	/*
	 * we should never arrive here...
	 */

	switch_text();
}
