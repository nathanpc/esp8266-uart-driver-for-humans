# Makefile
# ESP8266 project Makefile.
#
# Author: Nathan Campos <nathan@innoveworkshop.com>

# Project definitions.
NAME = sane-uart
VERSION = 0.1.0
PROJECT = $(NAME)-v$(VERSION)
SRC = example.c sane_uart.c driver/uart.c

# Serial communication.
PORT = /dev/ttyUSB0
BAUDRATE = 115200

# Xtensa compiler flags.
CC = xtensa-lx106-elf-gcc
CFLAGS = -iquote $(SRCDIR)/ -mlongcalls -DICACHE_FLASH
LDLIBS = -nostdlib -Wl,--start-group -lc -lgcc -lhal -lphy -lpp -lnet80211 -llwip -lwpa -lcrypto -lmain -ljson -lupgrade -lssl -lpwm -lsmartconfig -Wl,--end-group
LDFLAGS = -Teagle.app.v6.ld
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

# File directories.
SRCDIR = src
OBJDIR = build
DISTDIR = dist

all: $(DISTDIR)/$(PROJECT)-0x00000.bin

$(DISTDIR)/$(PROJECT)-0x00000.bin: $(OBJDIR)/$(PROJECT).elf
	esptool.py elf2image -o "$(DISTDIR)/$(PROJECT)-" $^

$(OBJDIR)/$(PROJECT).elf: $(OBJ)
	@mkdir -p "$(DISTDIR)"
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o "$(OBJDIR)/$(PROJECT).elf" $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p "$(OBJDIR)"
	@mkdir -p "$(OBJDIR)/driver"
	$(CC) -c $(CFLAGS) -o $@ $<

flash: $(DISTDIR)/$(PROJECT)-0x00000.bin
	esptool.py -p $(PORT) write_flash 0x00000 "$(DISTDIR)/$(PROJECT)-0x00000.bin" 0x10000 "$(DISTDIR)/$(PROJECT)-0x10000.bin"

monitor:
	screen -DRSq $(NAME) -t ESP8266 $(PORT) $(BAUDRATE)

clean:
	-screen -d $(NAME)
	rm -rf $(OBJDIR)
	rm -rf $(DISTDIR)

