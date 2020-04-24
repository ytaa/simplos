BIN=test2.bin
OUTPUT_DIR=bin/
INCLUDE = -Iinclude
LIBS = -nostdlib -lc -lgcc

CFLAGS?=-O2
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra $(UDEF)

SOURCES = $(wildcard ./src/*.c)
ASM_SOURCES = $(wildcard ./src/*.s)
OBJECTS = $(SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)
DEPS = $(wildcard ./include/*.h) $(wildcard ./include/*/*.h) $(wildcard ./include/*/*/*.h)

$(OUTPUT_DIR)$(BIN): $(OBJECTS)
	$(CC) -o $(OUTPUT_DIR)$(BIN) $(CFLAGS) $(OBJECTS) $(LIBS)
	rm src/*.o

%.o: %.s $(DEPS) 
	$(AS) $< -o $@

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ -std=gnu99 $(CFLAGS) $(INCLUDE)
