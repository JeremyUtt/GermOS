#pragma once
#include <stdint.h>

#include <string.hpp>
int initSerial();
int isTransmitEmpty();
void serialWriteChar(char a);
// void serialWriteStr(char* string);
// void serialWriteStr(const char* string);
void serialWriteStr(string str);