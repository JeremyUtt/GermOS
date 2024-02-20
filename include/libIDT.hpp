#pragma once

#define IDT_SIZE 256
struct interrupt_frame;
#define INTERRUPT __attribute__((interrupt))

struct IdtEntry {
    uint16_t offsetLower;
    uint16_t GdtSegment;
    uint8_t zero;
    uint8_t flags;
    uint16_t offsetHigher;
} __attribute__((packed));

struct IdtPointer {
    uint16_t limit;
    IdtEntry* base;
} __attribute__((packed));

extern IdtPointer idtPtr;

void idtInit();
void loadIdtEntry(int32_t isr_number, uint32_t base, uint16_t GdtSegment, uint8_t flags);
void printIdtEntry(uint8_t i);
extern "C" void loadIdt(void*);
