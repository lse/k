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
include config.mk

ROMS	= \
	  roms/chichehunter \
	  roms/chichepong \
	  roms/chichevaders \
	  roms/perrodlauncher \
	  roms/skate \
	  roms/yakanoid \


SUBDIRS	= \
	  $(ROMS) \
	  k \
	  libs/libc \
	  libs/libk \
	  tools/mkkfs \

ABS_INSTALL = $(abspath $(INSTALL_ROOT))

all: k.iso

k: libs/libc

$(ROMS): tools/mkkfs libs/libc libs/libk

k.iso: install
	./tools/create-iso.sh $@ $(INSTALL_ROOT) $(ROMS)

$(SUBDIRS):
	$(MAKE) -C $@

install: libs/libc libs/libk k
	mkdir -p $(ABS_INSTALL)
	for I in $(ROMS);			\
	do					\
		$(MAKE) INSTALL_ROOT=$(ABS_INSTALL) -C $$I $@ || exit 1;	\
	done
	$(MAKE) INSTALL_ROOT=$(ABS_INSTALL) -C k $@

clean:
	for I in $(SUBDIRS);			\
	do					\
		$(MAKE) -C $$I $@ || exit 1;	\
	done
	$(RM) k.iso 
	$(RM) -r root
	$(RM) -r iso

.PHONY: $(SUBDIRS) $(INSTALL_ROOT) install
