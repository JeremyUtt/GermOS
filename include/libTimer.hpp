#pragma once
#include <libIDT.hpp>
#include <stdint.h>
#define PIT_CHANNEL0 0x40  // PIT Channel 0's Data Register Port
#define PIT_CMD 0x43       // PIT Chip's Command Register Port

namespace Timer {
extern int timerPIT;
extern uint64_t globalTime;

void setEnabled(bool OnOff);
void setFreq(int hz);

}  // namespace Timer
void sleep(int milliseconds);
// extern "C" void incTimer();
extern "C" void asmTimerHandler();

// INTERRUPT void timerHandler(struct interrupt_frame*);