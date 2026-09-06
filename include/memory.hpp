#pragma once
#include <stdint.h>

void* malloc(uint16_t bytes);
void free(void* mem);
void initMem();
void printMem();
void getMemInfo();

// void operator delete(void* prt, unsigned long size) {
//     free(prt);
// }


// void operator delete(void* prt) {
//     free(prt);
// }