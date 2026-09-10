#pragma once
#include <stdint.h>
enum ProcessState { Initalized, Running, Queued, Paused, Exited };

struct CpuState {
    int eax;
    int ecx;
    int ebx;
    int edx;

    int esp;  // Stack Pointer
    int ebp;  // Stack Base Pointer

    int esi;  // Source Index register      (??)
    int edi;  // Destination Index register (??)

    int eip;  // Program Counter
    int eflags;

    int cs;  // Code segment selector
    int ss;  // Stack segment selector
    int ds;  // Data segment selector
    int es;  // Extra segment selector
    int fs;  // Extra segment selector
    int gs;  // Extra segment selector
};


extern CpuState currentState;
extern "C" uint8_t storeState(CpuState* address);
extern "C" uint8_t createFrame();
extern "C" void asmfunction();