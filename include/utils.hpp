#pragma once
#include <stdint.h>
#define size_t uint32_t

#include <string.hpp>
string intToStr(int value, int base);
uint32_t strToInt(string value);
uint32_t getStrLen(char String[]);
uint32_t getStrLen(const char String[]);
bool strcmp(char str1[], char str2[]);
uint32_t pow(uint32_t a, uint32_t b);
void memcpy(void* src, void* dst, size_t size);
template <typename left, typename right>
struct pair {
    left first;
    right second;
};
