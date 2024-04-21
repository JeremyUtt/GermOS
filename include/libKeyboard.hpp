#pragma once
#include <stdint.h>

#include <libIDT.hpp>

#define LeftShift 0x2A
#define RightShift 0x36
#define Enter 0x1C
#define BackSpace 0x0E
#define Escape 0x1b
#define Spacebar 0x39
#define Left 0x4b
#define Right 0x4d
#define Up 0x48
#define Down 0x50
#define Special 0xe0

namespace KB {
uint8_t getKeyBufferIndex();
void pushKeyBuffer(char chr);
char popKeyBuffer();

void init(void);
char Translate(uint8_t scancode, bool uppercase);

void waitForKeyboard();
}  // namespace KB

INTERRUPT void keyboardHandler(interrupt_frame* frame);