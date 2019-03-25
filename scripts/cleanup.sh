cd ..
rm -f kernel/src/*.o
rm -f kernel/bin/*
rm -f libc/*/*.o
rm -f libc/bin/*
if [ -d "sysroot" ]; then
	rm -rf sysroot
fi
mkdir sysroot
mkdir sysroot/usr
mkdir sysroot/usr/include
mkdir sysroot/usr/lib
mkdir sysroot/boot
