# simplos

This is a minimal operating system designed for x86 architecture developed as a project for my engineering degree. It is mainly developed for educational purpose.

## Getting Started

### Cross-compiler

To compile this system you will need to build a cross-compiler. Firstly you will need a Unix-like host system. In this example we will use Ubuntu 18.04.2 LTS. As a cross-complier we will use GCC configured for i686-elf target. To instal all necessary dependencies required for building GCC - type folowing into command line:

```bash
sudo apt install -y \
build-essential \
bison \
flex \
libgmp3-dev \
libmpc-dev \
libmpfr-dev \
texinfo
```
Next you will need to download the sources for [GNU Binutils](https://www.gnu.org/software/binutils) and [GCC](https://www.gnu.org/software/gcc). You can get them from offical project sites. You have to make sure that the versions of soruces are compatible with installed dependencies. The versions tested to work well with eachother are binutils-2.31.1 and gcc-6.5.0. 

After downloading and decompressing the sources you are ready to build your cross-compiler. First you need to chose where to install the complier. In this example we will youse $HOME/opt/cross. We will start with configuaring build for Binutils and then using generated makefiles to build and install them. Next we will do the same with GCC. An example of code that can build the cross-compiler is shown below.

```bash
export PATH="$HOME/opt/cross/bin:$PATH"
cd $HOME/src
mkdir binutils-build
cd binutils-build
../binutils-2.31.1/configure --target="i686-elf" --prefix="$HOME/opt/cross" --with-sysroot --disable-nls --disable-werror
make
make install

cd $HOME/src
mkdir gcc-build
cd gcc-build
../gcc-6.5.0/configure --target="i686-elf" --prefix="$HOME/opt/cross" --disable-nls --enable-languages=c --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
```

### Installing
Now you are ready to build the operating system. After cloning the repository, you can use bash scripts located in scripts directory. Make sure all the files are executable. To build the operating system update the create_env_variables.sh script to contain path to your cross-compiler and then run following commands:

```bash
cd scripts
./create_env_variables.sh
./cleanup.sh
./build.sh
```
After this ready compiled binary file containing the system will be located in kernel/bin directory.

You can run this binary on a emulator. This system is tested to work on quemu emulator. You can automaticly build and run simulation using sim.sh script.

