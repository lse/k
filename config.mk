CFLAGS	= -std=gnu99 -Os -Wall -Wextra -nostdinc -fno-builtin -ffreestanding \
	  -m32 -fno-asynchronous-unwind-tables -fno-common -static
# SSP causes compilation problems on Ubuntu
CFLAGS	+= -fno-stack-protector
#K_EXTRA_CFLAGS = -g3
#CFLAGS += -ffunction-sections -fdata-sections
CPPFLAGS += -I$(shell $(CC) -m32 --print-file-name=include)

ASFLAGS = -m32
LDFLAGS	= -nostdlib -m32 -Wl,--build-id=none -nostartfiles
#LDFLAGS += -Wl,--gc-sections -Wl,--print-gc-sections
ARFLAGS = src
