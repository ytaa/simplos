INCLUDE = -Iinclude

CFLAGS?=-O2 -g
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra $(UDEF)

SOURCES = $(wildcard ./*/*.c)
ASM_SOURCES = $(wildcard ./*/*.S)
OBJECTS = $(SOURCES:.c=.o) $(ASM_SOURCES:.S=.o)
DEPS = $(wildcard ./include/*.h)

bin/libc.a: $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)
	cp bin/libc.a $(DEVOS_SYSROOT)$(DEVOS_LIBDIR)

%.o: %.S $(DEPS) 
	$(CC) -c $< -o $@ -std=gnu99 $(CFLAGS) $(INCLUDE)

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ -std=gnu99 $(CFLAGS) $(INCLUDE)

clean:
	rm ./*/*.o bin/libc.a
