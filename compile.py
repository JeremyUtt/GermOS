import os
import glob

os.system('export PATH=$PATH:"/usr/local/i386elfgcc/bin"')
os.system('set -e')


print("\n====> assembling bootloader")
os.system('nasm "src/boot.asm" -f bin -o "build/boot.bin" ')

# print("===================================================================================")
print("\n====> assembling kernel entry...")
os.system('nasm "src/kernel_entry.asm" -f elf -o "build/kernel_entry.o"')

# print("===================================================================================")

print("\n====> Compiling C++...")

files = ""

for file in glob.glob("./src/*.cpp"):
    trimmed = file.split("/")[2]
    os.system(f'i386-elf-gcc -ffreestanding -m32 -g -c -mgeneral-regs-only -Wreturn-local-addr -mno-red-zone -I ./include {file} -o "build/{trimmed.split(".")[0]}.o"')
    files = files + "./build/" + trimmed.split(".")[0] + ".o "

for file in glob.glob("./src/*.c"):
    trimmed = file.split("/")[2]
    os.system(f'i386-elf-gcc -ffreestanding -m32 -g -c -mgeneral-regs-only -Wreturn-local-addr -mno-red-zone -I ./include {file} -o "build/{trimmed.split(".")[0]}.o"')
    files = files + "./build/" + trimmed.split(".")[0] + ".o "

print("\n====> Linking Kernal Files...")
os.system(f'i386-elf-ld -o "build/full_kernel.bin" -Ttext 0x1000 "build/kernel_entry.o" {files}  --oformat binary')

# For Debugger:
os.system(f'i386-elf-ld -o "build/full_kernel.o" -Ttext 0x1000 "build/kernel_entry.o" {files}')
os.system('objcopy --only-keep-debug build/full_kernel.o bin/full_kernel.sym')

print("\n====> Adding Bootloader and Finishing up...")
os.system('cat "build/boot.bin" "build/full_kernel.bin" > "build/everything.bin"')
os.system('cat "build/everything.bin" "src/zeros.bin" > "bin/OS.bin"')

print("\n====> Complete! file stored at ./OS.bin")
