qemu-system-i386 -drive format=raw,file="./bin/OS.bin",index=0,if=floppy, -m 128M -gdb tcp::1269 -serial telnet:localhost:6969,server &
# qemu-system-i386 -drive format=raw,file="./bin/OS.bin",index=0,if=floppy, -m 128M  -serial telnet:localhost:6969,server &
# qemu-system-i386 -drive format=raw,file="./bin/OS.bin",index=0,if=floppy, -m 128M 
sleep .5
telnet localhost 6969
wait