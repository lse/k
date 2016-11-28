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
#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include <kstd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* this file should really be cleaned :) */

/*
 * some characteristics about the display.
 */
#define GRAPHIC_WIDTH	320
#define GRAPHIC_HEIGHT	200
#define FB_SIZE	64000
#define PALETTE_SIZE	256

/*
 * image  structure. used  load_image, clear_image  and  draw_image to
 * manipulate images.
 */

typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned char **data;
} t_image;

/*
 * animation structure.
 *
 */

typedef struct {
	int nr_img;
	int current_img;
	unsigned long delay;
	unsigned long jiffies;
	t_image **imgs;
} t_anim;

/*
 * Windows BMP file header.
 */

typedef struct {
	char signature[2];
	unsigned long filesize;
	unsigned long reserved1;
	unsigned long offset;
	unsigned long reserved2;
	unsigned long width;
	unsigned long height;
	unsigned short planes;
	unsigned short bpp;
	unsigned long reserved3;
	unsigned long size;
	char reserved[16];
} __attribute__ ((packed)) t_bitmap_header;

/*
 * a color is an index in the palette.
 */
typedef unsigned int t_color;

/*
 * some colors.
 */
enum e_colors {
	BLACK = 0,
	WHITE = 255,
	RED = 249,
	GREEN = 250,
	YELLOW = 251,
	BLUE = 252,
	PURPLE = 253,
	AQUA = 254,
	ORANGE = 23
};

void switch_graphic(void);

void switch_text(void);

void draw_begin(void);

void draw_end(void);

void draw_clear(t_color color);

void draw_pixel(unsigned int x, unsigned int y, t_color color);

void draw_line(unsigned int x1, unsigned int y1,
	       unsigned int x2, unsigned int y2, t_color color);

void draw_rect(unsigned int x1, unsigned int y1,
	       unsigned int x2, unsigned int y2, t_color color);

void draw_fillrect(unsigned int x1, unsigned int y1,
		   unsigned int x2, unsigned int y2,
		   t_color color, t_color interior);

t_image *load_image(const char *path);

void clear_image(t_image * image);

void draw_image_alpha(t_image * image,
		      unsigned int x, unsigned int y, unsigned int alpha);

void draw_image(t_image * image, unsigned int x, unsigned int y);

void draw_text(const char *s,
	       unsigned int x, unsigned int y, t_color fg, t_color bg);

t_anim *load_anim(char *paths, int delay);

void draw_anim(t_anim * anim, int x, int y, unsigned long jiffies);

void blue_screen(const char *message);

#endif				/* !GRAPHIC_H_ */
