#include <stdio.h>
#include <kernel/tty.h>

int putchar(int ic) {
	char c = (char) ic;
	tty_put_char(c);
	return ic;
}
