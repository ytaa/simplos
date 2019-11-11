#!/bin/bash
export DEVOS_NAME="simplos"
#adding cross-compiler to path
export PATH="$HOME/opt/cross/bin:$PATH"
export DEVOS_ELF_TARGET=i686
export DEVOS_TARGET="$DEVOS_ELF_TARGET-elf"
export DEVOS_COMPILER="$DEVOS_TARGET-gcc"
export DEVOS_REPODIR="$HOME/Desktop/shared/studia/inżynierka/simplos"
export DEVOS_SYSROOT="$DEVOS_REPODIR/sysroot"
export DEVOS_INCLUDEDIR="/usr/include"
export DEVOS_LIBDIR="/usr/lib"

export CC="$DEVOS_TARGET-gcc" 
export AS="$DEVOS_TARGET-as"
export AR="$DEVOS_TARGET-ar"

export CC="$CC --sysroot=$DEVOS_SYSROOT"
export CC="$CC -isystem=$DEVOS_INCLUDEDIR"
