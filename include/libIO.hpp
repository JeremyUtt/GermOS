#pragma once
#include <stdint.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20
#define COM1 0x3f8
#define TEXT_CURSOR1 0x3d4
#define TEXT_CURSOR2 0x3d5

void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
uint8_t inb(uint16_t port);
void io_wait();

void pokeb(uint32_t address, uint32_t val);
uint8_t peekb(uint32_t address);
void pokew(uint32_t address, uint32_t val);