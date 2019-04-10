#include <stdio.h>
#include <kernel/tty.h>

int putchar(int ic) {
	char c = (char) ic;
	if(c == '\t'){
		for(int i=0; i<TTY_TAB_SIZE; i++){
			tty_put_char(' ');
		}
	}else{
		tty_put_char(c);
	}
	return ic;
}
