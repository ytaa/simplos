#!/bin/bash

if [ -z $1 ]
then
	echo 'ERROR: missing parameter'
	echo 'Specify target directory'
else
    ./libc.sh clean
    ./libc.sh
	cd $1
	make
    cd $DEVOS_SYSROOT/../scripts
    ./libc.sh clean
fi
