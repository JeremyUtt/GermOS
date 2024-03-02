#include <stdint.h>

#include <converts.hpp>
#include <libIO.hpp>
#include <libSerial.hpp>
#include <libTimer.hpp>
#include <system.hpp>

namespace Timer {
int timerPIT = 0;

void setEnabled(bool OnOff) {
    uint8_t currentMask = inb(PIC1_DATA);
    if (OnOff) {
        outb(PIC1_DATA, currentMask & 0b11111110);
    } else {
        outb(PIC1_DATA, currentMask | 0b00000001);
    }
}

void setFreq(int hz) {
    int divisor = 1193180 / hz;
    outb(PIT_CMD, 0x36);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, divisor >> 8);
}
}  // namespace Timer

void sleep(int milliseconds) {
    Timer::timerPIT = 0;
    while (Timer::timerPIT < milliseconds) {
        halt();
    }
}

INTERRUPT void timerHandler(struct interrupt_frame*) {
    Timer::timerPIT++;
    // serialWriteStr("Hello World!");
    outb(PIC1_COMMAND, PIC_EOI);
    return;
}