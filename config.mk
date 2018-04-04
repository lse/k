CFLAGS	= -std=gnu99 -Os -Wall -Wextra -nostdinc -fno-builtin -ffreestanding \
	  -m32 -fno-asynchronous-unwind-tables -fno-common -fno-pie -march=i486
# SSP causes compilation problems on Ubuntu
CFLAGS	+= -fno-stack-protector
#K_EXTRA_CFLAGS = -g3
# Place each function or data item into a separate section
CFLAGS += -ffunction-sections -fdata-sections
CPPFLAGS += -I$(shell $(CC) -m32 --print-file-name=include)
# enable dlmalloc self-corruption tests.
#CPPFLAGS += -DDEBUG=1 -DFOOTERS=1

ASFLAGS = -m32
LDFLAGS = -nostdlib -m32 -Wl,--build-id=none -nostartfiles -static
# Detect and remove unused sections while linking the objects
LDFLAGS += -Wl,--gc-sections
#LDFLAGS += -Wl,--print-gc-sections
ARFLAGS = src
INSTALL = install -C -D
INSTALL_ROOT ?= iso
