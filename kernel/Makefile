BIN=$$DEVOS_NAME.bin
INCLUDE = -Iinclude
LIBS = -nostdlib -lc -lgcc

CFLAGS?=-O2
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra 

SOURCES = $(wildcard ./src/*.c)
OBJECTS = $(SOURCES:.c=.o)
DEPS = $(wildcard ./include/*.h)
DEPS := $(DEPS) $(wildcard ./include/*/*.h)

bin/$(BIN): $(OBJECTS) src/boot.o
	$(CC) -T src/linker.ld -o bin/$(BIN) $(CFLAGS) $(OBJECTS) src/boot.o $(LIBS)

src/boot.o: src/boot.s
	$(AS) $< -o $@

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ -std=gnu99 $(CFLAGS) $(INCLUDE)
