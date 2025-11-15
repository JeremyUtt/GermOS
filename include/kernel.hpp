#pragma once
#include <libGUI.hpp>
extern const int16_t KERNEL_SIZE;

void initKernel(Renderer& renderer);
char* checkKernelMemory(uint32_t start, uint32_t len, string toFind);

void startTUI();
void startGUI();