#!/bin/bash

if [ -z $1 ]
then
	echo 'ERROR: missing parameter'
	echo 'Specify target device (probably /dev/sdb1)'
	echo "Run 'ls /dev/sd*' before and after inserting USB stick to determine device name"
else

	cd ../kernel/iso
	sudo dd if=$DEVOS_NAME.iso of=$1 && sync

fi
