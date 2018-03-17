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

struct skater skater;
struct anim *anim[7];
int score = 0;
int score_tmp = 0;

static void skater_anim(void)
{
	if (skater.trick_delay)
		skater.trick_delay--;

	if (skater.fly) {
		if (!skater.trick_delay)
			skater.trick = TRICK_NONE;
	} else if (!skater.trick_delay) {
		switch (get_park(skater.current_box)) {
		case BOX_UP:
			skater.trick = TRICK_UP;
			break;
		case BOX_RAIL:
			if (skater.slide)
				break;
			/* Falls through. */
		default:
			if (scrolling.freq < 6)
				skater.trick = TRICK_RUN;
			else
				skater.trick = TRICK_NONE;
			break;
		}
	}

	draw_anim(graphics[skater.trick].gfx, skater.x, skater.y - skater.height, jiffies);
}

void skater_jump(int height)
{
	if (!skater.fly && !skater.jump)
		skater.jump = skater.y - 14 - 8 * ((6 - height));
}

void skater_flip(void)
{
	if (skater.trick_delay || !skater.fly)
		return;

	skater.rotation = 0;

	skater.trick_delay = 20;
	skater.trick = TRICK_FLIP;
	skater.combo++;
	score_tmp += 20;
}

void skater_rotate(void)
{
	if (skater.trick_delay || !skater.fly)
		return;

	skater.trick_delay = 25;
	skater.trick = TRICK_ROTATE;
	skater.rotation += 360;
	skater.combo++;
	score_tmp += 30;
}

static void skater_crash(void)
{
	skater.trick_delay = 100;
	skater.trick = TRICK_CRASH;

	skater.rotation = 0;
	score_tmp = skater.combo = 0;
}

void skater_slide(void)
{
	if (skater.trick_delay || get_park(skater.current_box) != BOX_RAIL)
		return;

	if (scrolling.freq > 5)
		return;

	skater.slide = 1;

	if (skater.trick != TRICK_SLIDE && !skater.fly)
		skater.combo++;

	skater.trick = TRICK_SLIDE;
	score_tmp += 3;
}

void skater_init(void)
{
	skater.x = 0;
	skater.y = GRAPHIC_HEIGHT - 10;
	skater.height = 50;
	skater.width = 50;

	skater.fly = 0;
	skater.jump = 0;
	skater.trick_delay = 0;
	skater.trick = TRICK_RUN;
	skater.slide = 0;
	skater.rotation = 0;
	skater.combo = 0;
	score_tmp = 0;
}

int skater_height(int pos)
{
	int y_max = 0;
	int x_max = pos - 6;
	int x;
	int y;

	for (x = pos - 6; x < pos + 6; x++) {
		y = box_height(get_box(x), get_shift(x));
		if (y > y_max) {
			y_max = y;
			x_max = x;
		}
	}
	return x_max;
}

void skater_height_update(int floor_height)
{
	if (skater.jump) {
		skater.y -= 2;
		if (skater.y <= skater.jump)
			skater.jump = 0;
	} else {
		if (skater.y < floor_height)
			skater.y += 2;
		else
			skater.y = floor_height;
	}

	skater.fly = (skater.y < floor_height) ? 1 : 0;

	if (!skater.fly)
		skater.rotation = 0;

	if (!skater.fly && skater.trick_delay && skater.trick != TRICK_CRASH)
		skater_crash();
}

void skater_draw(void)
{
	int floor;
	int max_x;

	max_x = skater_height(skater.x + skater.width / 2 + scrolling.x);

	skater.current_box = get_box(max_x);
	if (get_park(skater.current_box) != BOX_RAIL)
		skater.slide = 0;

	floor = GRAPHIC_HEIGHT - 10 - box_height(skater.current_box, get_shift(max_x));

	skater_height_update(floor);
	skater_anim();

	if (!skater.trick_delay && !skater.slide && !skater.fly) {
		if (skater.trick != TRICK_CRASH) {
			score += score_tmp;
			if (skater.combo)
				score += (10 + skater.combo) * (skater.combo - 1);
			skater.combo = 0;
			score_tmp = 0;
		}
	}
}
