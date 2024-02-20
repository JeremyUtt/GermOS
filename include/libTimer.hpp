#pragma once
#include <libIDT.hpp>

void setTimerStatus(bool OnOff);

INTERRUPT void timerHandler(struct interrupt_frame*);