#pragma once
#include <process.hpp>
#include <stdint.h>
void zeroCpuState(CpuState* state);
void simulateTasks();
void task();
void trampolineFunction();

void cStoreState(CpuState* destination, InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot);
void cLoadState(CpuState* source, InterruptFrame* interruptFrame, RegisterSnapshot* registerSnapshot);
uint32_t* initializeTaskStack(uint8_t* stack, uint32_t stackSize, uint32_t entrypoint);