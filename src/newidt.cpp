// https://stackoverflow.com/questions/37618111/keyboard-irq-within-an-x86-kernel
#include "newidt.hpp"

#include "io.hpp"

#define IDT_SIZE 256

idt_entry idt_table[IDT_SIZE];
idt_pointer idt_ptr;

void loadIdtEntry(int isr_number, unsigned long base, short int selector,
                  unsigned char flags) {
    idt_table[isr_number].offset_lowerbits = base & 0xFFFF;
    idt_table[isr_number].offset_higherbits = (base >> 16) & 0xFFFF;
    idt_table[isr_number].selector = selector;
    idt_table[isr_number].flags = flags;
    idt_table[isr_number].zero = 0;
}

static void initialize_idt_pointer() {
    idt_ptr.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idt_ptr.base = (unsigned int)&idt_table;
}

static void initialize_pic() {
    /* ICW1 - begin initialization */
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    /* ICW2 - remap offset address of idt_table */
    /*
     * In x86 protected mode, we have to remap the PICs beyond 0x20 because
     * Intel have designated the first 32 interrupts as "reserved" for cpu
     * exceptions
     */
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);

    /* ICW3 - setup cascading */
    outb(PIC1_DATA, 0x00);
    outb(PIC2_DATA, 0x00);

    /* ICW4 - environment info */
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    /* Initialization finished */

    /* mask interrupts */
    outb(0x21, 0xff);
    outb(0xA1, 0xff);
}

void idtInit() {
    initialize_pic();
    initialize_idt_pointer();
    load_idt(&idt_ptr);
}