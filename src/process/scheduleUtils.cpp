#include "scheduleUtils.hpp"

#include <memory.hpp>
#include <printf.hpp>
#include <scheduler.hpp>
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

// void simulateTasks() {
//     processTemp newProcess;
//     newProcess.entryPoint = (uint32_t)task;
//     newProcess.stackSize = 500;
//     newProcess.stackStart = (uint8_t*)malloc(500);
//     newProcess.name = "Temp Process";
//     newProcess.uiMode = TEXT;
//     zeroCpuState(&newProcess.state);

//     uint32_t* stackPointer = initializeTaskStack(newProcess.stackStart, newProcess.stackSize, (uint32_t)trampolineFunction);

//     newProcess.state.ebp = (uint32_t)newProcess.stackStart;
//     newProcess.state.esp = (uint32_t)stackPointer;

//     free(newProcess.stackStart);
// }

void task() {
    while (true) {
        fprintf(Serial, "Hello World");
    }
}

void trampolineFunction() {
    while (true) {
        fprintf(Serial, "trampolineFunction");
    }
}
