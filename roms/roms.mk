include ../../config.mk

CPPFLAGS += -MMD -I ../../k/include -I ../../libs/libc/include -I ../../libs/libk/include
LDFLAGS += -Wl,-T../roms.lds
LDLIBS	= -L ../../libs/libk -L ../../libs/libc -lk -lc

DEPS = $(OBJS:.o=.d)

MKKFS	= ../../tools/mkkfs/mkkfs

all: $(TARGET).rom

$(TARGET): $(OBJS)

clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET) $(TARGET).rom

$(TARGET).rom: $(TARGET)
	$(MKKFS) -o $@ -n $(ROM_TITLE) $(ROM_FILES)

-include $(DEPS)
