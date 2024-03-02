#pragma once
#include <libIDT.hpp>

#define PIT_CHANNEL0 0x40  // PIT Channel 0's Data Register Port
#define PIT_CMD 0x43       // PIT Chip's Command Register Port

namespace Timer {
extern int timerPIT;

void setEnabled(bool OnOff);
void setFreq(int hz);

}  // namespace Timer
void sleep(int milliseconds);
INTERRUPT void timerHandler(struct interrupt_frame*);