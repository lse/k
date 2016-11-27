include ../../config.mk

CPPFLAGS += -MMD -I ../../k/include -I ../../sdk/libc/include -I ../../sdk/libk/include
LDFLAGS += -Wl,-T../roms.lds
LDLIBS	= -L ../../sdk/libk -L ../../sdk/libc -lk -lc

DEPS = $(OBJS:.o=.d)

MKKFS	= ../../sdk/mkkfs/mkkfs

all: $(TARGET).rom

$(TARGET): $(OBJS)

clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET) $(TARGET).rom

$(TARGET).rom: $(TARGET)
	$(MKKFS) -r $@ -n $(ROM_TITLE) -f $(ROM_FILES)

-include $(DEPS)
