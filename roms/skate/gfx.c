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
	{TRICK_NONE, 7, "/skater1.bmp /skater2.bmp", NULL},
	{TRICK_RUN, 7, "/skater3.bmp /skater4.bmp /skater5.bmp /skater6.bmp", NULL},
	{TRICK_FLIP, 5, "/flip1.bmp /flip2.bmp /flip3.bmp /flip4.bmp /flip5.bmp", NULL},
	{TRICK_UP, 4, "/skaterup.bmp", NULL},
	{TRICK_SLIDE, 6, "/slide1.bmp /slide2.bmp", NULL},
	{TRICK_CRASH, 6, "/crash.bmp", NULL},
	{TRICK_ROTATE, 8, "/rotate1.bmp /rotate2.bmp /rotate3.bmp /rotate4.bmp /rotate5.bmp", NULL},
	{BOX_UP, 0, "/boxup.bmp", NULL},
	{BOX, 0, "/box.bmp", NULL},
	{BOX_RAIL, 0, "/rail.bmp", NULL},
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
