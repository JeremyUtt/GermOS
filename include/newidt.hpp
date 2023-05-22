#pragma once



//could go in cpp file instead
// (with reference here for idt_pointer and idt&ptr)
//===========================================
struct idt_entry{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short int offset_higherbits;
} __attribute__((packed));

struct idt_pointer{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));
//============================================
//reference:

// struct idt_pointer;
extern idt_pointer idt_ptr;

//=========

void idtInit();
void loadIdtEntry(int isr_number, unsigned long base, short int selector, unsigned char flags);
extern "C" void load_idt(void*);
