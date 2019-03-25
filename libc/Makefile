INCLUDE = -Iinclude

CFLAGS?=-O2 -g
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra 

SOURCES = $(wildcard ./*/*.c)
OBJECTS = $(SOURCES:.c=.o)
DEPS = $(wildcard ./include/*.h)

bin/libc.a: $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)
	cp bin/libc.a $(DEVOS_SYSROOT)$(DEVOS_LIBDIR)

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ -std=gnu99 $(CFLAGS) $(INCLUDE)
