#!/bin/bash
cd ..
cd libc
make
cp -r include ../sysroot/usr
