#pragma once
int initSerial();
int is_transmit_empty();
void serialWriteChar(char a);
void serialWriteStr(char* string);
void serialWriteStr(const char* string);