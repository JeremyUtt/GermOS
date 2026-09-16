#pragma once
#include <stdint.h>
#include <process.hpp>
#include <printf.hpp>

struct processTemp {
    int16_t pid;
    string name;
    UiMode uiMode;
    CpuState state;
    ProcessState processState;
    
    uint32_t entryPoint;
    uint8_t* stackStart;
    uint16_t stackSize;
};


class scheduler {
  private:
    const static int PROCESS_COUNT = 5;
    const static int MAX_STACK_SIZE = 500;
    processTemp processes[PROCESS_COUNT];
    processTemp* currentProcess = nullptr;
    bool isProcess[PROCESS_COUNT] = {false};
    public:
    scheduler();
    ~scheduler();
    
    void printProcessInfo(stream to);
    int16_t createProcess(string name, uint32_t entrypoint, UiMode uiMode);
    void schedulerTick(InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot);
};

extern scheduler* schedulerInstance;

void testScheduler();