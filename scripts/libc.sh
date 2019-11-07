#!/bin/bash
cd ..
cd libc
make $1
cp -r include ../sysroot/usr
