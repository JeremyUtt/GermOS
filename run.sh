
normal(){
    qemu-system-i386 \
        -drive format=raw,file="./bin/OS.bin",index=0,if=floppy, \
        -m 128M \
        -gdb tcp::1269 \
        -serial telnet:localhost:6969,server &
    sleep .5
    telnet localhost 6969
    wait
}


debug() {
    qemu-system-i386 \
        -drive format=raw,file="./bin/OS.bin",index=0,if=floppy, \
        -m 128M \
        -gdb tcp::1269  \
        -chardev stdio,id=seabios \
        -device isa-debugcon,iobase=0x402,chardev=seabios \
        -bios /home/jeremy/GitRepos/seabios/out/bios.bin
        # -S

}


if [ "$1" == "debug" ]; then
    debug
else
    normal
fi