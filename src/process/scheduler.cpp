#include "scheduler.hpp"
#include <process.hpp>

CpuState currentState;


extern "C" int determineIfSwitchNeeded(){
    
    currentState.eax = 0;
    currentState.ecx = 0;
    currentState.edx = 0;
    currentState.ebx = 0;

    currentState.esp = 0;
    currentState.ebp = 0;

    currentState.esi = 0;
    currentState.edi = 0;

    currentState.eip = 0;
    currentState.eflags = 0;

    currentState.cs = 0x8;
    currentState.ss = 0;
    currentState.ds = 0;
    currentState.es = 0;
    currentState.fs = 0;
    currentState.gs = 0;
    // Check if a context switch is needed based on the current state of the CPU and the scheduler's state.
    // This function should return true if a context switch is required, false otherwise.

    // For demonstration purposes, let's assume we always want to switch for now.
    return 1;
}