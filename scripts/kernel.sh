#!/bin/bash
cd ..
cd kernel
make
cp -r include ../sysroot/usr
