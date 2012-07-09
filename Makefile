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

K_PATH		= ./k
ROMS_PATH	= ./roms
SDK_PATH	= ./sdk

SUBDIRS		= $(K_PATH) $(ROMS_PATH) $(SDK_PATH)

K		= $(K_PATH)/k
ROM		= $(ROMS_PATH)/rom

GAME		?= skate

FLOPPY		= k_floppy
USBSTICK	= k_usbstick

.PHONY: k sdk rom

all: k

sdk:
	$(MAKE) -C $(SDK_PATH)

k: sdk
	$(MAKE) -C $(K_PATH)

rom: sdk
	$(MAKE) -C $(ROMS_PATH) GAME=$(GAME)

clean:
	for I in $(SUBDIRS);			\
	do					\
		$(MAKE) -C $$I $@ || exit 1;	\
	done

distclean:
	for I in $(SUBDIRS);			\
	do					\
		$(MAKE) -C $$I $@ || exit 1;	\
	done
	$(RM) $(FLOPPY) $(USBSTICK)

boot: floppy
	$(QEMU) -fda $(FLOPPY) -soundhw all -net none &

floppy: k rom
	$(CP) sdk/grub/floppy $(FLOPPY)
	$(MCOPY) -i $(FLOPPY) $(K) ::/modules/k
	$(MCOPY) -i $(FLOPPY) $(ROM) ::/modules/rom

usbstick: k rom
	$(CP) sdk/grub/usbstick $(USBSTICK)
	$(MOUNT) -o loop $(USBSTICK) $(MNT)
	$(CP) $(K) $(MNT)/modules/k
	$(CP) $(ROM) $(MNT)/modules/rom
	$(UMOUNT) $(MNT)
