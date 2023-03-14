qemu-system-i386 -drive format=raw,file="./bin/OS.bin",index=0,if=floppy, -m 128M -gdb tcp::1234 -serial telnet:localhost:4321,server &
sleep .25
telnet localhost 4321
wait