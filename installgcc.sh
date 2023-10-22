# nasm and qemu
sudo apt install nasm
sudo apt install qemu
sudo apt-get install qemu-kvm

# GCC cross compiler for i386 systems (might take quite some time, prepare food)

sudo apt update
sudo apt install build-essential
sudo apt install bison
sudo apt install flex
sudo apt install libgmp3-dev
sudo apt install libmpc-dev
sudo apt install libmpfr-dev
sudo apt install texinfo

export PREFIX="/opt/i386elfgcc"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"
export GCCVERSION="13.2.0"
export UTILVERSION="2.41"
mkdir /tmp/src
cd /tmp/src
curl -O http://ftp.gnu.org/gnu/binutils/binutils-$UTILVERSION.tar.gz
tar xf binutils-$UTILVERSION.tar.gz
mkdir binutils-build
cd binutils-build
../binutils-$UTILVERSION/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
sudo make all install 2>&1 | tee make.log

cd /tmp/src
curl -O https://ftp.gnu.org/gnu/gcc/gcc-$GCCVERSION/gcc-$GCCVERSION.tar.gz
tar xf gcc-$GCCVERSION.tar.gz
mkdir gcc-build
cd gcc-build
echo Configure: . . . . . . .
../gcc-$GCCVERSION/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-language=c++ --without-headers
echo MAKE ALL-GCC:
sudo make all-gcc -j16
echo MAKE ALL-TARGET-LIBGCC:
sudo make all-target-libgcc -j16
echo MAKE INSTALL-GCC:
sudo make install-gcc -j16
echo MAKE INSTALL-TARGET-LIBGCC:
sudo make install-target-libgcc -j16
echo HERE U GO MAYBE:
ls /opt/i386elfgcc/bin
export PATH="$PATH:/usr/local/i386elfgcc/bin"