#!/bin/bash

if [ -z $1 ]
then
	echo 'ERROR: missing parameter'
	echo 'Specify target directory in program root directory'
else
    separator="---------------------------------------------------"

    echo
    echo "libc clean:"
    echo $separator
    ./libc.sh clean
    echo $separator

    echo
    echo "libc user build:"
    echo $separator
    ./libc.sh
    echo $separator

    echo
    echo "$1 build:"
    echo $separator
	cd $DEVOS_REPODIR/programs/$1
	make
    echo $separator

    echo
    echo "$1 install:"
    echo $separator
    cd $DEVOS_REPODIR/scripts
    ./binConverter/binConverter $1/bin/$1.bin $1
    echo $separator

    echo
    echo "libc clean:"
    echo $separator
    ./libc.sh clean
    echo $separator
fi
