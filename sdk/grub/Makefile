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

STAGES	= /boot/grub/stage1 /boot/grub/stage2
MENU_LST= menu.lst
GRUBSFPP= grubs-floppy
GRUBSUSB= grubs-usb
GRUB	= grub
FLOPPY	= floppy
USBSTICK= usbstick
MNT	= /mnt

all: floppy usbstick

floppy::
	$(MKFS) -t msdos -C $@ 1440
	$(MMD) -i $@ ::/boot
	$(MMD) -i $@ ::/boot/grub
	$(MMD) -i $@ ::/modules
	$(MCOPY) -i $@ $(STAGES) $(MENU_LST) ::/boot/grub/
	$(CAT) $(GRUBSFPP) | $(GRUB) --batch --device-map=/dev/null

usbstick::
	$(DD) if=/dev/zero of=$@ bs=512 count=3K
	$(MKFS) -t ext3 -F $@
	$(MOUNT) -o loop $@ $(MNT)
	$(MKDIR) $(MNT)/boot $(MNT)/boot/grub $(MNT)/modules
	$(CP) $(STAGES) $(MENU_LST) $(MNT)/boot/grub
	$(UMOUNT) $(MNT)
	$(CAT) $(GRUBSUSB) | $(GRUB) --batch --device-map=/dev/null

clean:
	$(RM) $(FLOPPY) $(USBSTICK)
