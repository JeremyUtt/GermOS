#pragma once
int init_serial();
static int is_transmit_empty();
void serialWriteChar(char a);
void serialWriteStr(char* string);