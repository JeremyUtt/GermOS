#pragma once
#include <stdint.h>
int pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
