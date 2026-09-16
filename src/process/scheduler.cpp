#include "scheduler.hpp"

#include <libTimer.hpp>
#include <memory.hpp>

#include "scheduleUtils.hpp"
// #include <process.hpp>

scheduler* schedulerInstance = nullptr;

scheduler::scheduler() {
}

scheduler::~scheduler() {
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

int16_t scheduler::createProcess(string name, uint32_t entrypoint, UiMode uiMode) {
    for (int16_t i = 0; i < PROCESS_COUNT; i++) {
        if (this->isProcess[i]) {
            continue;
        }
        this->isProcess[i] = true;
        this->processes[i].entryPoint = entrypoint;
        this->processes[i].stackSize = MAX_STACK_SIZE;
        this->processes[i].stackStart = (uint8_t*)malloc(MAX_STACK_SIZE);
        this->processes[i].name = name;
        this->processes[i].uiMode = uiMode;
        this->processes[i].processState = Initalized;
        this->processes[i].pid = i;
        zeroCpuState(&this->processes[i].state);

        uint32_t* stackPointer = initializeTaskStack(this->processes[i].stackStart, this->processes[i].stackSize, (uint32_t)trampolineFunction);

        this->processes[i].state.ebp = (uint32_t)this->processes[i].stackStart;
        this->processes[i].state.esp = (uint32_t)stackPointer;

        return i;
    }
    return -1;
}

void scheduler::printProcessInfo(stream to) {
    for (size_t i = 0; i < this->PROCESS_COUNT; i++) {
        fprintf(to, "Entry %d: %s\n", i, this->isProcess[i] ? processes[i].name : "No Process");
        // if (this->isProcess[i]) {
        //     continue;
        // }
    }
}


void testScheduler(){
    schedulerInstance->printProcessInfo(Serial);
    int16_t pid = schedulerInstance->createProcess("Test Process", (uint32_t)task, TEXT);
    fprintf(Serial, "pid: %d\n", pid);
    
    schedulerInstance->printProcessInfo(Serial);
    
    

}


extern "C" void schedulerTick(InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot) {
    Timer::timerPIT++;
    Timer::globalTime++;
    schedulerInstance->schedulerTick(interruptFrame, registerSnapshot);
}
