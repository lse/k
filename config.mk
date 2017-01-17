CFLAGS	= -Os -Wall -Wextra -nostdinc -fno-builtin -ffreestanding -m32 \
	  -fno-asynchronous-unwind-tables -fno-common -static
# SSP causes compilation problems on Ubuntu
CFLAGS	+= -fno-stack-protector
#CFLAGS += -g3 -save-temps
CPPFLAGS += -I$(shell $(CC) -m32 --print-file-name=include)

ASFLAGS = -m32
LDFLAGS	= -nostdlib -m32 -Wl,--build-id=none -nostartfiles
ARFLAGS = src
