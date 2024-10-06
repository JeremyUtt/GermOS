// https://stackoverflow.com/questions/37618111/keyboard-irq-within-an-x86-kernel
#include <stdint.h>

#include <libIDT.hpp>
#include <libIO.hpp>
#include <printf.hpp>
#include <utils.hpp>

IdtEntry idtTable[IDT_SIZE];
IdtPointer idtPtr;

void loadIdtEntry(int32_t isrNumber, uint32_t baseAddress, uint16_t gdtSegment, uint8_t flags) {
    idtTable[isrNumber].offsetLower = baseAddress & 0xFFFF;
    idtTable[isrNumber].offsetHigher = (baseAddress >> 16) & 0xFFFF;
    idtTable[isrNumber].GdtSegment = gdtSegment;
    idtTable[isrNumber].flags = flags;
    idtTable[isrNumber].zero = 0;
}

static void initIdtPtr() {
    idtPtr.limit = (sizeof(IdtEntry) * IDT_SIZE) - 1;
    idtPtr.base = idtTable;
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
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

void idtInit() {
    initPic();
    initIdtPtr();
    loadIdt(&idtPtr);
}

void printIdtEntry(uint8_t i) {
    fprintf(Serial, "IDT Entry: 0x%X\n", i);
    fprintf(Serial, "\tOffset: 0x%X\n", (idtTable[i].offsetHigher << 16) + idtTable[i].offsetLower);
    fprintf(Serial, "\tGdt Segment: %d\n", idtTable[i].GdtSegment);
    fprintf(Serial, "\tFlags: 0b%d\n", idtTable[i].flags);
}
