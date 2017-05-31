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
#define FB_SIZE		(GRAPHIC_WIDTH * GRAPHIC_HEIGHT)
#define PALETTE_SIZE	256

/*
 * image  structure. used  load_image, clear_image  and  draw_image to
 * manipulate images.
 */

struct image {
	unsigned int width;
	unsigned int height;
	unsigned char **data;
};

/*
 * animation structure.
 *
 */

struct anim {
	int nr_img;
	int current_img;
	unsigned long delay;
	unsigned long jiffies;
	struct image **imgs;
};


/*
 * a color is an index in the palette.
 */
typedef unsigned int color_t;

/*
 * some colors.
 */
enum colors {
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

/*
 * this function switches to graphic mode.
 */
void switch_graphic(void);

/*
 * this function get back to text mode.
 */
void switch_text(void);

/*
 * this fumction changes the color palette of the VGA.
 */
void set_palette(unsigned int *new_palette, size_t size);

/*
 * call this function at the beginning of drawing a frame.
 */
void draw_begin(void);

/*
 * call this function when finished drawing. this is the function that
 * copy your buffered draw from off-screen buffer to framebuffer.
 */
void draw_end(void);

/*
 * clears the screen with given color.
 */
void draw_clear(color_t color);

/*
 * this function plot a pixel of given color at given position.
 */
void draw_pixel(unsigned int x, unsigned int y, color_t color);

/*
 * draw a line.
 */
void draw_line(unsigned int x1, unsigned int y1,
	       unsigned int x2, unsigned int y2, color_t color);

/*
 * draw an empty rectangle.
 */
void draw_rect(unsigned int x1, unsigned int y1,
	       unsigned int x2, unsigned int y2, color_t color);

/*
 * draw a solid rectangle.
 */
void draw_fillrect(unsigned int x1, unsigned int y1,
		   unsigned int x2, unsigned int y2,
		   color_t color, color_t interior);

/*
 * load a Windows BITMAP (BMP) from file.
 * the only supported files are 8 bits per pixels paletted.
 * the only supported palette is the default one (obtained with Paint).
 */
struct image *load_image(const char *path);

/*
 * destroy a loaded image.
 */
void clear_image(struct image *image);

/*
 * display a loaded image with transparency.
 */
void draw_image_alpha(struct image *image,
		      unsigned int x, unsigned int y, unsigned int alpha);

/*
 * display a loaded image.
 */
void draw_image(struct image *image, unsigned int x, unsigned int y);

/*
 * draw some text.
 */
void draw_text(const char *s,
	       unsigned int x, unsigned int y, color_t fg, color_t bg);

/*
 * load an animation.
 * paths is string containing the images name separated by a space.
 * load_anim supports the same image formats as load_image.
 * delay is the displaying time of each image (in ticks).
 *
 * invocation example: load_anim("pic1 pic2 pic3 pic4 pic5", PIC_ANIM_DELAY);
 */
struct anim *load_anim(char *paths, int delay);

/*
 * draw an animation at coordinates (x, y)
 *
 * jiffies is the reference ticks counter which should
 * be incremented at every timer tick.
 */
void draw_anim(struct anim * anim, int x, int y, unsigned long jiffies);

/*
 * video blue screen.
 */
extern void (*blue_screen)(const char *message);

#endif				/* !GRAPHIC_H_ */
