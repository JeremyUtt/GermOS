#pragma once
#include <stdint.h>

void* malloc(uint16_t bytes);
void free(void* mem);
void initMem();
void printMem();