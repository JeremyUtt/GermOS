#include "scheduler.hpp"

#include <libTimer.hpp>
#include <memory.hpp>

#include "scheduleUtils.hpp"
#include <system.hpp>
// #include <process.hpp>

scheduler* schedulerInstance = nullptr;

scheduler::scheduler() {
    // process 0 is a special case representing the kernel. values like stackStart etc will be unused; 
    isProcess[0] = true;
    processes[0].entryPoint = 0;
    processes[0].name = "Kernel";
    processes[0].pid = 0;
    processes[0].previousState = Initalized;
    processes[0].processState = Running;
    processes[0].stackSize = 0;
    processes[0].stackStart = nullptr;
    processes[0].startTime = 0;
    // processes[0].state not saved yet
    processes[0].stopTime = 0;
    processes[0].uiMode = TEXT;

    currentProcess = &processes[0];
}

scheduler::~scheduler() {
    for (size_t i = 0; i < PROCESS_COUNT; i++) {
        if (this->isProcess[i]) {
            free(this->processes[i].stackStart);
        }
    }
}

processTemp* scheduler::chooseNextTask() {
    // Current Algorithm: choose first QUEUED task

    for (size_t i = 0; i < PROCESS_COUNT; i++) {
        if (!this->isProcess[i]) {
            continue;
        }
        if (this->processes[i].processState == Queued){
            return &this->processes[i];
        }
    }
    return nullptr;
}

void scheduler::schedulerTick(InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot) {
    uint64_t interruptTime = Timer::globalTime;
    
    if(interruptTime - lastSwitch < MIN_TIME_SLICE_MS){
        // fprintf(Serial, "Waiting %d\n", Timer::globalTime);
        return;
    }

    processTemp* nexttask = chooseNextTask();
    if (nexttask==nullptr){
        return;
    }
    
    if(currentProcess->pid == nexttask->pid){
        return;
    }


    zeroCpuState(&currentProcess->CPUstate);
    cStoreState(&currentProcess->CPUstate, interruptFrame, registerSnapshot);
    currentProcess->previousState = Running;
    currentProcess->processState = Queued;
    currentProcess->uiMode = getUiMode();
    currentProcess->stopTime = interruptTime;
    this->lastSwitch = interruptTime;

    currentProcess = nexttask;
    currentProcess->previousState = Queued;
    currentProcess->processState = Running;
    currentProcess->startTime = interruptTime;
    setUiMode(currentProcess->uiMode);
    cLoadState(&currentProcess->CPUstate, interruptFrame, registerSnapshot);

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
    disableInterrupts();
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
        this->processes[i].processState = Queued;
        this->processes[i].pid = i;
        // zeroCpuState(this->processes[i].CPUstate);

        uint32_t* stackPointer = initializeTaskStack(this->processes[i].stackStart, this->processes[i].stackSize, (uint32_t)trampolineFunction);

        this->processes[i].CPUstate.ebp = (uint32_t)this->processes[i].stackStart;
        this->processes[i].CPUstate.esp = (uint32_t)stackPointer;
        this->processes[i].CPUstate.eip = (uint32_t)entrypoint;
        this->processes[i].CPUstate.eflags = 0x202;
        this->processes[i].CPUstate.cs = 0x8;
        enableInterrupts();
        return i;
    }
    enableInterrupts();
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

// hopefully equivalent to the asm version storeState
// does NOT save segment registers other than CS
void scheduler::cStoreState(CpuState* destination, InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot) {
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

void scheduler::cLoadState(CpuState* source, InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot) {
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

uint32_t* scheduler::initializeTaskStack(uint8_t* stack, uint32_t stackSize, uint32_t entrypoint) {
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

void testScheduler() {
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
