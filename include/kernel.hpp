#pragma once
extern const int KERNEL_SIZE;
void initKernel();
char* checkKernelMemory(uint32_t start, uint32_t len, string toFind);

void startTUI();
void startGUI();