include ../../config.mk

DEPS = $(OBJS:.o=.d)

MKKFS	= ../../tools/mkkfs/mkkfs

all: $(TARGET)

$(TARGET): CPPFLAGS += -MMD -I ../../k/include -I ../../libs/libc/include -I ../../libs/libk/include -DRES_PATH='"/usr/$(TARGET)/"'
$(TARGET): LDFLAGS += -Wl,-T../roms.lds
$(TARGET): LDLIBS = -L ../../libs/libk -L ../../libs/libc -lk -lc
$(TARGET): $(OBJS)

install: $(TARGET)
	$(INSTALL) $(TARGET) $(INSTALL_ROOT)/bin/$(TARGET)
	for i in $(ROM_FILES); do \
		$(INSTALL) -m0644 $$i $(INSTALL_ROOT)/usr/$(TARGET)/$$i || exit 1; \
	done

clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET) $(TARGET).rom

-include $(DEPS)

.PHONY: install clean all
