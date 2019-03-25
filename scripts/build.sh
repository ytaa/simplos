#!/bin/bash
cp -r ../kernel/include ../sysroot/usr
bash libc.sh
bash kernel.sh
#cd ..
#cd kernel
#rm -f src/*.o
#rm -f bin/*
