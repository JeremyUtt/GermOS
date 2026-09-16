#pragma once
#include <stdint.h>
#include <process.hpp>

struct processTemp {
    string name;
    UiMode uiMode;
    CpuState state;
    uint32_t entryPoint;
    uint8_t* stackStart;
    uint16_t stackSize;
};


class scheduler {
  private:
    const static int PROCESS_COUNT = 5;
    processTemp processes[PROCESS_COUNT];
    processTemp* currentProcess = nullptr;
    bool isProcess[PROCESS_COUNT] = {false};

  public:
    scheduler();
    ~scheduler();

    void createProcess(string name, uint32_t entrypoint, UiMode uiMode);
    void schedulerTick(InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot);
};

extern scheduler* schedulerInstance;