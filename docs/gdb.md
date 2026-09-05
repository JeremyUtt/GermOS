# Using GDB


## Starting GDB CLI

```
cd ~/Dev/germos
gdb bin/OS.sym
target remote :1269
```

```
layout next
layout asm
p [var name]
step [amount]   - souece code lines
next [amount]   - machine code lines 
```

examime memory (eg stack trace)
```
x/64xb $sp-16

x/ [num] [heX/Bin/Dec] [Bytes/Words] space [start addr]
x/ [num] [base display] [group len] space [start addr]
```

## Sources
- https://stackoverflow.com/questions/39762989/how-to-watch-memory-access-with-gdb-on-guest-machine-in-qemu
- https://github.com/rohanrhu/gdb-frontend#installing
- https://wiki.osdev.org/GDB
- https://www.youtube.com/watch?v=bWH-nL7v5F4
- https://wiki.osdev.org/Kernel_Debugging
- https://sourceware.org/gdb/download/onlinedocs/gdb/Continuing-and-Stepping.html 
- https://developer.apple.com/library/archive/documentation/DeveloperTools/gdb/gdb/gdb_8.html 
- https://www.youtube.com/watch?v=kONFFecp3R8

---

list
// shows source code


