ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

TARGET	:= libds4wiibt.a
INCLUDE := bt_includes

CFLAGS	:= -O2 -Wall $(MACHDEP) -I$(LIBOGC_INC) -I$(INCLUDE)
CFILES  := $(wildcard *.c)
OFILES  := $(CFILES:.c=.o)
INC_INSTALL := ds4wiibt.h bt_includes/*.h

all: $(TARGET)
	@echo "Type 'make install' to install"

$(TARGET): $(OFILES)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: all
	@echo install ...
	cp $(TARGET) $(LIBOGC_LIB)
	cp $(INC_INSTALL) $(LIBOGC_INC)

clean:
	@echo clean ...
	@rm -fr $(OFILES) $(TARGET)
