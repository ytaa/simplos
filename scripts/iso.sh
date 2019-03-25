#!/bin/bash
bash build.sh
cd ../kernel
if grub-file --is-x86-multiboot bin/$DEVOS_NAME.bin; then 
	echo MULTIBOOT: OK
	cp bin/$DEVOS_NAME.bin iso/boot/$DEVOS_NAME.bin
	rm -f iso/$DEVOS_NAME.iso
	grub-mkrescue -o iso/$DEVOS_NAME.iso iso
else 
	echo MULTIBOOT: FAIL
	exit 10
fi
