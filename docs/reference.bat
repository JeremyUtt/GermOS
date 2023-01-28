cd C:\Users\Jeremy Utt\Documents\Projects\os test\
C:\"Program Files"\NASM\nasm -f bin "reference.asm" -o "C:\Users\Jeremy Utt\Documents\Projects\os test\reference.bin"
cd C:\"Program Files"\qemu\
qemu-system-x86_64 "C:\Users\Jeremy Utt\Documents\Projects\os test\reference.bin" 
@REM -gdb tcp:localhost:6969 -S