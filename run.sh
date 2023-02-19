qemu-system-i386.exe -drive format=raw,file="./bin/OS.bin",index=0,if=floppy,  -m 128M -gdb tcp::1234 -serial telnet:localhost:4321,server,nowait &
# sleep 1s 
# telnet localhost 4321 &