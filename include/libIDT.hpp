#pragma once

#define IDT_SIZE 256

struct IdtEntry{
    uint16_t offsetLower;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t offsetHigher;
} __attribute__((packed));

struct IdtPointer{
    uint16_t limit;
    IdtEntry* base;
} __attribute__((packed));

extern IdtPointer idtPtr;

void idtInit();
void loadIdtEntry(int32_t isr_number, uint32_t base, uint16_t selector, uint8_t flags);
extern "C" void loadIdt(void*);
