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
#ifndef LIBVGA_H
#define LIBVGA_H

#include <stddef.h>

#define VGA_AC_INDEX		0x3C0
#define VGA_AC_WRITE		0x3C0
#define VGA_AC_READ		0x3C1
#define VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define VGA_DAC_MASK		0x3C6
#define VGA_DAC_READ_INDEX	0x3C7
#define VGA_DAC_WRITE_INDEX	0x3C8
#define VGA_DAC_DATA		0x3C9
#define VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF

#define VGA_CRTC_INDEX		0x3D4
#define VGA_CRTC_DATA		0x3D5
#define VGA_INSTAT_READ	0x3DA

#define VGA_NUM_SEQ_REGS	5
#define VGA_NUM_CRTC_REGS	25
#define VGA_NUM_GC_REGS	9
#define VGA_NUM_AC_REGS	21

void libvga_set_palette(unsigned int *new_palette, size_t size);

char *libvga_get_framebuffer(void);

void libvga_switch_mode13h(void);

void libvga_switch_mode3h(void);

#endif				/* !LIBVGA_H */
