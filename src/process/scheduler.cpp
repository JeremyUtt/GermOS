#include "scheduler.hpp"
#include <process.hpp>
#include <printf.hpp>
CpuState currentState;

void zeroCpuState(CpuState* state){
    state->eax = 0;
    state->ecx = 0;
    state->edx = 0;
    state->ebx = 0;
    state->esp = 0;
    state->ebp = 0;
    state->esi = 0;
    state->edi = 0;
    state->eip = 0;
    state->eflags = 0;
    state->cs = 0x8;
    state->ss = 0;
    state->ds = 0;
    state->es = 0;
    state->fs = 0;
    state->gs = 0;
}



extern "C" int determineIfSwitchNeeded(){
    zeroCpuState(&currentState);
    // Check if a context switch is needed based on the current state of the CPU and the scheduler's state.
    // This function should return true if a context switch is required, false otherwise.

    // For demonstration purposes, let's assume we always want to switch for now.
    return 1;
}


// hopefully equivalent to the asm version storeState
// does NOT save segment registers other than CS
extern "C" void cStoreState(CpuState* destination, InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot){
    destination->cs = interruptFrame->cs;
    destination->eflags = interruptFrame->eflags;
    destination->eip = interruptFrame->ip;
    
    destination->eax = registerSnapshot->eax;
    destination->ebx = registerSnapshot->ebx;
    destination->ecx = registerSnapshot->ecx;
    destination->edx = registerSnapshot->edx;
    
    destination->esp = registerSnapshot->esp;
    destination->ebp = registerSnapshot->ebp;
    
    destination->esi = registerSnapshot->esi;
    destination->edi = registerSnapshot->edi;
    
    //hardcode segment registers
    // not really needed. just being verbose
    destination->ds = 0;
    destination->es = 0;
    destination->fs = 0;
    destination->gs = 0;
    destination->ss = 0;
    
    return;
}


extern "C" void cLoadState(CpuState* source, InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot){
    interruptFrame->cs = source->cs;
    interruptFrame->eflags = source->eflags;
    interruptFrame->ip = source->eip;

    registerSnapshot->eax = source->eax;
    registerSnapshot->ebx = source->ebx;
    registerSnapshot->ecx = source->ecx;
    registerSnapshot->edx = source->edx;

    registerSnapshot->ebp = source->ebp;
    registerSnapshot->esp = source->esp;

    registerSnapshot->esi = source->esi;
    registerSnapshot->edi = source->edi;
}


// extern "C" void schedulerTick(InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot){
//     cLoadState(currentState, interruptFrame, registerSnapshot);

// }