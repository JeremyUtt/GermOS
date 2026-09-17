#pragma once
#include <stdint.h>

#include <printf.hpp>
#include <process.hpp>

struct processTemp {
    int16_t pid;
    string name;
    UiMode uiMode;
    CpuState CPUstate;
    ProcessState processState;
    uint32_t entryPoint;
    uint8_t* stackStart;
    uint16_t stackSize;
    
    ProcessState previousState;
    uint64_t startTime = 0;
    uint64_t stopTime = 0;
};

class scheduler {
  private:
    const static int PROCESS_COUNT = 5;
    const static int MAX_STACK_SIZE = 500;
    const static int MIN_TIME_SLICE_MS = 1000;
    processTemp processes[PROCESS_COUNT];
    processTemp* currentProcess = nullptr;
    bool isProcess[PROCESS_COUNT] = {false};
    void cStoreState(CpuState* destination, InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot);
    void cLoadState(CpuState* source, InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot);
    uint32_t* initializeTaskStack(uint8_t* stack, uint32_t stackSize, uint32_t entrypoint);
    processTemp* chooseNextTask();
  
    uint64_t lastSwitch = 0;
  
    public:
    scheduler();
    ~scheduler();

    void printProcessInfo(stream to);
    int16_t createProcess(string name, uint32_t entrypoint, UiMode uiMode);
    void schedulerTick(InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot);

    
  
};

extern scheduler* schedulerInstance;

void testScheduler();