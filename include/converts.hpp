#pragma once
#include <string.hpp>
#include <stdint.h>
string intToStr(int value, int base);
uint32_t strToInt(string value);
uint32_t getStrLen(char String[]);
uint32_t getStrLen(const char String[]);
bool strcmp(char str1[], char str2[]);
uint32_t pow(uint32_t a, uint32_t b);