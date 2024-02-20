#include <stdint.h>

#include <libIO.hpp>
#include <libSerial.hpp>
#include <libTimer.hpp>
void setTimerStatus(bool OnOff) {
    uint8_t currentMask = inb(PIC1_DATA);
    if (OnOff) {
        outb(PIC1_DATA, currentMask & 0b11111110);
    } else{
        outb(PIC1_DATA, currentMask | 0b00000001);
    }
}

INTERRUPT void timerHandler(struct interrupt_frame*) {
    serialWriteStr("Hello World!");
    outb(PIC1_COMMAND, PIC_EOI);
}
