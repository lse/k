#
# Copyright (c) LSE
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY LSE AS IS AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL LSE BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
include ../../config.mk

TARGET	  = rom_prog
OBJS	  = skate.o	\
	    park.o	\
	    skater.o	\
	    gfx.o
ROM_FILE  = ../rom
ROM_TITLE = "skate"
ROM_FILES = $(TARGET) boxup.bmp box.bmp rail.bmp skater1.bmp skater2.bmp	\
	skater3.bmp skater4.bmp skater5.bmp skater6.bmp	skaterup.bmp		\
	flip1.bmp flip2.bmp flip3.bmp flip4.bmp flip5.bmp slide1.bmp		\
	slide2.bmp crash.bmp rotate1.bmp rotate2.bmp rotate3.bmp rotate4.bmp	\
	rotate5.bmp logo.bmp

CFLAGS	+= -I ../../sdk/libk -I ../../sdk/libc
LDFLAGS	+= -L ../../sdk/libk -L ../../sdk/libc -lk -lc -T ../roms.lds

MKKFS	= ../../sdk/mkkfs/mkkfs

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(TARGET)

rom: $(TARGET)
	$(MKKFS) -r $(ROM_FILE) -n $(ROM_TITLE) -f $(ROM_FILES)
