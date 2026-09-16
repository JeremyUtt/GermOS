#include <scheduler.hpp>
#include "scheduleUtils.hpp"
#include <memory.hpp>
#include <printf.hpp>
void zeroCpuState(CpuState* state) {
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

// hopefully equivalent to the asm version storeState
// does NOT save segment registers other than CS
void cStoreState(CpuState* destination, InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot) {
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

    // hardcode segment registers
    //  not really needed. just being verbose
    destination->ds = 0;
    destination->es = 0;
    destination->fs = 0;
    destination->gs = 0;
    destination->ss = 0;

    return;
}

void cLoadState(CpuState* source, InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot) {
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

uint32_t* initializeTaskStack(uint8_t* stack, uint32_t stackSize, uint32_t entrypoint) {
    uint32_t* sp = (uint32_t*)(stack + stackSize);

    // CPU iret frame, pushed in reverse order.
    *--sp = 0x202;       // EFLAGS
    *--sp = 0x08;        // CS
    *--sp = entrypoint;  // EIP

    // Value discarded by the handler's "add esp, 4".
    *--sp = 0;  // saved original ESP

    // Match the handler's pop order.
    *--sp = 0;  // EBP
    *--sp = 0;  // EAX
    *--sp = 0;  // EBX
    *--sp = 0;  // ECX
    *--sp = 0;  // EDX
    *--sp = 0;  // ESI
    *--sp = 0;  // EDI

    return sp;
}


void simulateTasks() {
    processTemp newProcess;
    newProcess.entryPoint = (uint32_t)task;
    newProcess.stackSize = 500;
    newProcess.stackStart = (uint8_t*)malloc(500);
    newProcess.name = "Temp Process";
    newProcess.uiMode = TEXT;
    zeroCpuState(&newProcess.state);

    uint32_t* stackPointer = initializeTaskStack(newProcess.stackStart, newProcess.stackSize, (uint32_t)trampolineFunction);

    newProcess.state.ebp = (uint32_t)newProcess.stackStart;
    newProcess.state.esp = (uint32_t)stackPointer;

    free(newProcess.stackStart);
}

void task() {
    printf("Hello World");
}

void trampolineFunction() {
    while (true) {
        fprintf(Serial, "trampolineFunction");
    }
}
