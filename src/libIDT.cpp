// https://stackoverflow.com/questions/37618111/keyboard-irq-within-an-x86-kernel
#include <libIO.hpp>
#include <libIDT.hpp>
#include <stdint.h>

IdtEntry idtTable[IDT_SIZE];
IdtPointer idtPtr;



void loadIdtEntry(int32_t isr_number, uint32_t base, uint16_t selector,
                  uint8_t flags) {
    idtTable[isr_number].offsetLower = base & 0xFFFF;
    idtTable[isr_number].offsetHigher = (base >> 16) & 0xFFFF;
    idtTable[isr_number].selector = selector;
    idtTable[isr_number].flags = flags;
    idtTable[isr_number].zero = 0;
}

static void initIdtPtr() {
    idtPtr.limit = (sizeof(IdtEntry) * IDT_SIZE) - 1;
    idtPtr.base = &idtTable[0];
}

static void initPic() {
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
    initPic();
    initIdtPtr();
    loadIdt(&idtPtr);
}