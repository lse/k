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

struct monster {
	int x;
	int y;
	int alive;
};

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

	playsound(intro, -1);

	if (!imgchef || !imgchiche)
		blue_screen("Unable to load chiche.bmp");

	while (k != KEY_ENTER) {

		k = getkey();
		if (k == KEY_UP)
			sel = 1;

		if (k == KEY_DOWN)
			sel = 0;

		draw_begin();
		draw_image(imgchiche, 5, 10);
		draw_image(imgchef, 5, 100);
		draw_text("   ChicheVaders   ", 160, 50, RED, 0);
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
		t = gettick();
		while (gettick() - t < 66) ;
	}

	playsound(NULL, -1);

	clear_sound(intro);
	clear_image(imgchef);
	clear_image(imgchiche);
	return sel;
}

#define WIDTH 320

#define MONSTERS_COLS 6
#define NB_MONSTERS 1		//24
#define X_INIT_MONSTERS 10
#define X_GAP_MONSTERS 25
#define MONSTER_X_BBOX 20
#define MONSTER_Y_BBOX 24

#define SEINE_W_X_BBOX 20
#define SEINE_W_Y_BBOX 24

#define P_SIZE 15
#define NUM_MOVE 10
/*
 * game loop.
 */

static void game_loop(struct image * img)
{
	unsigned long t;
	int p;
	int w, dw;
	int wx, wy;
	int k;
	int x, y;
	int dx, dy;
	int mdx, mdy, mcnt;
	struct monster monsters[NB_MONSTERS];
	int ym = X_INIT_MONSTERS;
	int i = 0;
	int good = 0;
	int standby = 0;
	int fin = 0;
	int win = 0;
	int dead_monsters = 0;
	int kill_seine_w = 0;
	struct image *imgw = load_image(RES_PATH "/res/seine_w.bmp");
	struct melody *sound = load_sound(RES_PATH "/res/ball.csf");

	if (!img)
		blue_screen("Unable to load chiche.bmp");

	p = 160;
	w = 2;
	dw = 2;
	wx = -1;
	wy = -1;
	x = -1;
	y = -1;
	dx = 0;
	dy = -5;
	mdx = 1;
	mdy = 2;
	mcnt = 0;

	/*
	 * init monsters.
	 */
	for (i = 0; i < NB_MONSTERS; i++) {
		if (i % MONSTERS_COLS == 0)
			ym += X_GAP_MONSTERS;
		monsters[i].y = ym;
		monsters[i].x =
		    15 + (i % MONSTERS_COLS) * (WIDTH / MONSTERS_COLS);
		monsters[i].alive = 1;
	}

	while (1) {
		t = gettick();

		draw_begin();

		k = getkey();
		if (k > 0) {
			if (k == KEY_ESC)
				standby = !standby;

			if ((standby || fin) && k == KEY_ENTER)
				return;

			if (!standby && !fin) {
				if (k == KEY_LEFT && p > (P_SIZE / 2 + 5))
					p -= 5;
				if (k == KEY_RIGHT
				    && p < (WIDTH - (P_SIZE / 2 + 5)))
					p += 5;

				if ((k == KEY_SPACE) && x == -1 && y == -1) {
					x = p;
					y = 200 - (P_SIZE / 3);
				}
			}
		}

		/*
		 * move and draw seine_w.
		 */
		if (!standby && !fin) {
			if (w >= 319 - MONSTER_X_BBOX || w <= 1)
				dw = -dw;
			w += dw;
			draw_image(imgw, w, 2);
		}

		/*
		 * draw, move and inpact seine_w bullet.
		 */
		if (wx != -1 || wy != -1) {
			if (!standby && !fin) {
				wx += dx;
				wy -= dy;

				if (wx >= (p - (P_SIZE / 2))
				    && wx <= (p + (P_SIZE / 2))
				    && wy >= (198 - (P_SIZE / 2))) {
					playsound(sound, 1);
					fin = 1;
					win = 0;
				}
			}

			if (wy >= 200) {
				wx = -1;
				wy = -1;
			} else
				draw_line(wx, wy, wx, wy + 2, RED);
		} else if (w >= (p - (P_SIZE / 2)) && w <= (p + (P_SIZE / 2))) {
			wx = w;
			wy = SEINE_W_Y_BBOX;
		}

		/*
		 * move and draw monsters.
		 */
		for (i = 0; i < NB_MONSTERS; i++) {
			if (i % MONSTERS_COLS == 0)
				mdx = -mdx;

			if (monsters[i].alive) {
				if (!standby && !fin) {
					if (mcnt == NUM_MOVE)
						monsters[i].y += mdy;
					monsters[i].x += mdx;

					if (monsters[i].y + MONSTER_Y_BBOX >=
					    200) {
						fin = 1;
						win = 0;
					}
				}
				draw_image(img, monsters[i].x, monsters[i].y);
			}
		}

		if (++mcnt > NUM_MOVE) {
			mdx = -mdx;
			mcnt = 0;
		}

		/*
		 * draw screen borders
		 */
		draw_line(0, 0, 319, 0, 64);	/* H 1 */
		draw_line(0, 199, 319, 199, 64);	/* H 2 */
		draw_line(0, 0, 0, 199, 64);	/* V 1 */
		draw_line(319, 0, 319, 199, 64);	/* V 2 */

		/*
		 * draw player.
		 */
		draw_fillrect(p - (P_SIZE / 2), 198 - (P_SIZE / 2),
			      p + (P_SIZE / 2), 198, WHITE, WHITE);
		draw_line(p, 198 - P_SIZE, p, 198, WHITE);

		/*
		 * draw, move and inpact bullet.
		 */
		if (x != -1 || y != -1) {

			if (!kill_seine_w && x >= w && x <= (w + SEINE_W_X_BBOX)
			    && y < SEINE_W_Y_BBOX + 20) {
				draw_line(w, SEINE_W_Y_BBOX + 5,
					  w + SEINE_W_X_BBOX,
					  SEINE_W_Y_BBOX + 5, BLUE);
				draw_line(w, 0, w, SEINE_W_Y_BBOX + 5, BLUE);
				draw_line(w + SEINE_W_X_BBOX, 0,
					  w + SEINE_W_X_BBOX,
					  SEINE_W_Y_BBOX + 5, BLUE);
			}

			good = 0;
			if (!standby && !fin) {
				x += dx;
				y += dy;
				for (i = 0; i < NB_MONSTERS; i++) {
					if (monsters[i].alive
					    && (x >= monsters[i].x
						&& x <=
						(monsters[i].x +
						 MONSTER_X_BBOX))
					    && (y >= monsters[i].y
						&& y <=
						(monsters[i].y +
						 MONSTER_Y_BBOX))) {
						playsound(sound, 1);
						dead_monsters++;
						monsters[i].alive = 0;
						good = 1;
						break;
					}
				}

				if (dead_monsters == NB_MONSTERS)
					kill_seine_w = 1;

				if (kill_seine_w
				    && (x >= w && x <= (w + SEINE_W_X_BBOX)
					&& y <= SEINE_W_Y_BBOX)) {
					playsound(sound, 1);
					fin = 1;
					win = 1;
				}
			}
			if (good || y <= 0) {
				x = -1;
				y = -1;
			} else
				draw_line(x, y, x, y + 2, WHITE);
		}

		/*
		 * draw PAUSE.
		 */
		if (standby && !fin) {
			draw_text("PAUSE", 135, 50, WHITE, RED);
			draw_text("ESC key to continue", 90, 90, WHITE, RED);
			draw_text("ENTER key to quit", 90, 130, WHITE, RED);
		}

		/*
		 * draw FIN.
		 */
		if (fin) {
			draw_text(win ? "YOU WIN!" : "YOU LOOSE!", 110, 70,
				  WHITE, RED);
			draw_text("ENTER key to quit", 90, 130, WHITE, RED);
		}

		draw_end();
		/*
		 * 33 ms sync between each frame.
		 */

		while (gettick() - t <= 33) ;
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
