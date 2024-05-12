// https://stackoverflow.com/questions/37618111/keyboard-irq-within-an-x86-kernel
#include <stdint.h>

#include <utils.hpp>
#include <libIDT.hpp>
#include <libIO.hpp>
#include <libSerial.hpp>

IdtEntry idtTable[IDT_SIZE];
IdtPointer idtPtr;

void loadIdtEntry(int32_t isr_number, uint32_t base, uint16_t GdtSegment, uint8_t flags) {
    idtTable[isr_number].offsetLower = base & 0xFFFF;
    idtTable[isr_number].offsetHigher = (base >> 16) & 0xFFFF;
    idtTable[isr_number].GdtSegment = GdtSegment;
    idtTable[isr_number].flags = flags;
    idtTable[isr_number].zero = 0;
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
    serialWriteStr("\n\rIDT Entry: 0x");
    serialWriteStr(intToStr(i, 16));
    serialWriteStr("\r\n\tOffset: 0x");
    serialWriteStr(intToStr((idtTable[i].offsetHigher << 16) + idtTable[i].offsetLower, 16));
    serialWriteStr("\r\n\tGdt Segment: ");
    serialWriteStr(intToStr(idtTable[i].GdtSegment, 10));
    serialWriteStr("\r\n\tFlags: 0b");
    serialWriteStr(intToStr(idtTable[i].flags, 2));
}