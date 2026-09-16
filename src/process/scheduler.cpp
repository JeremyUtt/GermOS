#include "scheduler.hpp"
#include "scheduleUtils.hpp"
#include <libTimer.hpp>
#include <memory.hpp>
#include <printf.hpp>
// #include <process.hpp>

scheduler* schedulerInstance = nullptr;

scheduler::scheduler(){

}

scheduler::~scheduler(){

}
void scheduler::schedulerTick(InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot) {
    // if state change needed
        // zeroCpuState(&currentState);
        // cStoreState(&currentState, interruptFrame, registerSnapshot);
        // save currentState to processes class instance

        // calculate next task using algorithm
        // get saved state from class instance
        // cLoadState(newState, interruptFrame, registerSnapshot);
    // endif

}

void scheduler::createProcess(string name, uint32_t entrypoint, UiMode uiMode) {
    for (size_t i = 0; i < PROCESS_COUNT; i++) {
        if (this->isProcess[i]) {
            continue;
        }
        this->processes[i].entryPoint = (uint32_t)task;
        this->processes[i].stackSize = 500;
        this->processes[i].stackStart = (uint8_t*)malloc(500);
        this->processes[i].name = "Temp Process";
        this->processes[i].uiMode = uiMode;
        zeroCpuState(&this->processes[i].state);

        uint32_t* stackPointer = initializeTaskStack(this->processes[i].stackStart, this->processes[i].stackSize, (uint32_t)trampolineFunction);

        this->processes[i].state.ebp = (uint32_t)this->processes[i].stackStart;
        this->processes[i].state.esp = (uint32_t)stackPointer;
    }
}


extern "C" void schedulerTick(InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot) {   
    Timer::timerPIT++;
    schedulerInstance->schedulerTick(interruptFrame, registerSnapshot);
}
