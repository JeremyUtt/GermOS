#pragma once
#include <string.hpp>
#include <stdint.h>
int initSerial();
int isTransmitEmpty();
void serialWriteChar(char a);
// void serialWriteStr(char* string);
// void serialWriteStr(const char* string);
void serialWriteStr(string str);