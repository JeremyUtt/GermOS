#! /usr/bin/python3
import os
import glob

os.system('export PATH=$PATH:"/usr/local/i386elfgcc/bin"')
os.system('set -e')


print("====> assembling bootloader")
os.system('nasm "src/boot.S" -f bin -o "build/boot.bin" ')

# print("===================================================================================")
print("====> assembling kernel entry...")
os.system('nasm "src/kernel_entry.asm" -f elf -o "build/kernel_entry.o"')

# print("===================================================================================")

print("====> Compiling C++...")

files = ""

for file in glob.glob("./src/*.cpp"):
    trimmed = file.split("/")[2]
    #           i386-elf-gcc -ffreestanding -m32 -g -c -mgeneral-regs-only -Wreturn-local-addr -O0 -mno-red-zone -I ./include -o build/pci.o src/pci.cpp
    os.system(f'/usr/local/i386elfgcc/bin/i386-elf-g++ -ffreestanding -m32 -g -c -O0 -mgeneral-regs-only -Wall -mno-red-zone -I ./include {file} -o "build/{trimmed.split(".")[0]}.o"')
    files = files + "./build/" + trimmed.split(".")[0] + ".o "

for file in glob.glob("./src/*.c"):
    trimmed = file.split("/")[2]
    os.system(f'/usr/local/i386elfgcc/bin/i386-elf-gcc -ffreestanding -m32 -g -c -O0 -mgeneral-regs-only -Wreturn-local-addr -mno-red-zone -I ./include {file} -o "build/{trimmed.split(".")[0]}.o"')
    files = files + "./build/" + trimmed.split(".")[0] + ".o "




# os.system(f'objcopy -O elf32-i386 -B i386 -I binary ./fonts/Uni2-Terminus12x6.psf ./build/Uni2-Terminus12x6.o"')


# for file in glob.glob("./fonts/*.psf"):
#     trimmed = file.split("/")[2]
#     os.system(f'objcopy -O elf32-i386 -B i386 -I binary {file} ./build/{trimmed.split(".")[0]}.o"')
#     files = files + "./build/" + trimmed.split(".")[0] + ".o "






os.system("objcopy -O elf32-i386 -B i386 -I binary fonts/Uni2-Terminus12x6.psf build/Uni2-Terminus12x6.o")



print("====> Linking Kernal Files...")
os.system(f'/usr/local/i386elfgcc/bin/i386-elf-ld -o "build/full_kernel.bin" -Ttext 0x1000 "build/kernel_entry.o" "build/Uni2-Terminus12x6.o" {files} --oformat binary')

# For Debugger:
os.system(f'/usr/local/i386elfgcc/bin/i386-elf-ld -o "build/full_kernel.o" -Ttext 0x1000 "build/kernel_entry.o" "build/Uni2-Terminus12x6.o" {files} ')
os.system('objcopy --only-keep-debug build/full_kernel.o bin/full_kernel.sym')

print("====> Adding Bootloader and Finishing up...")
os.system('cat "build/boot.bin" "build/full_kernel.bin" > "build/everything.bin"')
os.system('cat "build/everything.bin" "src/zeros.bin" > "bin/OS.bin"')

print("====> Complete! file stored at ./OS.bin")
