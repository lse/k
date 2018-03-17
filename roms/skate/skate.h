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
#ifndef SKATE_H_
#define SKATE_H_

#include <graphic.h>
#include <sound.h>
#include <stdlib.h>

enum e_gfx {
	TRICK_NONE = 0,
	TRICK_RUN,
	TRICK_FLIP,
	TRICK_UP,
	TRICK_SLIDE,
	TRICK_CRASH,
	TRICK_ROTATE,
	BOX_UP,
	BOX,
	BOX_RAIL,
	NO_BOX
};

struct skater {
	int x;
	int y;
	int height;
	int width;
	int fly;
	int jump;
	int trick_delay;
	unsigned int trick;
	int slide;
	int rotation;
	int combo;
	int current_box;
};

struct scrolling {
	unsigned long timer1;
	unsigned long timer2;
	int freq;
	int step;
	int x;
	int current_box;
	int width;
};

struct gfx {
	enum e_gfx id;
	int delay;
	char *filename;
	void *gfx;
};

#define BG_COLOR	BLACK
#define BOX_WIDTH	40
#define BOX_HEIGHT	40
#define SPEED		2

extern struct scrolling scrolling;
extern unsigned long jiffies;
extern struct skater skater;
extern int score;
extern int score_tmp;
extern struct gfx graphics[];
extern char *trick[];

int scroll_slowdown(void);
int scroll_speedup(void);
void scroll(void);

enum e_gfx get_park(size_t box);
void park_init(void);
void park_draw(void);
int box_height(int box, int shift);
int get_box(int x);
int get_shift(int x);

void skater_init(void);
void skater_draw(void);
void skater_jump(int height);
void skater_flip(void);
void skater_rotate(void);
void skater_slide(void);

void gfx_init(void);

#endif				/* !SKATE_H_ */
