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

struct gfx graphics[] = {
	{TRICK_NONE, 7, RES_PATH "/res/skater1.bmp " RES_PATH "/res/skater2.bmp", NULL},
	{TRICK_RUN, 7, RES_PATH "/res/skater3.bmp " RES_PATH "/res/skater4.bmp " RES_PATH "/res/skater5.bmp " RES_PATH "/res/skater6.bmp", NULL},
	{TRICK_FLIP, 5, RES_PATH "/res/flip1.bmp " RES_PATH "/res/flip2.bmp " RES_PATH "/res/flip3.bmp " RES_PATH "/res/flip4.bmp " RES_PATH "/res/flip5.bmp", NULL},
	{TRICK_UP, 4, RES_PATH "/res/skaterup.bmp", NULL},
	{TRICK_SLIDE, 6, RES_PATH "/res/slide1.bmp " RES_PATH "/res/slide2.bmp", NULL},
	{TRICK_CRASH, 6, RES_PATH "/res/crash.bmp", NULL},
	{TRICK_ROTATE, 8, RES_PATH "/res/rotate1.bmp " RES_PATH "/res/rotate2.bmp " RES_PATH "/res/rotate3.bmp " RES_PATH "/res/rotate4.bmp " RES_PATH "/res/rotate5.bmp", NULL},
	{BOX_UP, 0, RES_PATH "/res/boxup.bmp", NULL},
	{BOX, 0, RES_PATH "/res/box.bmp", NULL},
	{BOX_RAIL, 0, RES_PATH "/res/rail.bmp", NULL},
	{0, 0, NULL, NULL}
};

void gfx_init(void)
{
	for (int i = 0; graphics[i].filename; i++) {
		if (graphics[i].delay)
			graphics[i].gfx = load_anim(graphics[i].filename, graphics[i].delay);
		else
			graphics[i].gfx = load_image(graphics[i].filename);
	}
}
