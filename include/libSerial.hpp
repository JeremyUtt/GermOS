#pragma once
int initSerial();
int isTransmitEmpty();
void serialWriteChar(char a);
void serialWriteStr(char* string);
void serialWriteStr(const char* string);