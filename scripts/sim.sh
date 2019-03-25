#!/bin/bash
bash build.sh
cd ../kernel
if grub-file --is-x86-multiboot bin/$DEVOS_NAME.bin; then 
	echo MULTIBOOT: OK
	qemu-system-i386 -kernel bin/$DEVOS_NAME.bin
else 
	echo MULTIBOOT: FAIL
	exit 10
fi
