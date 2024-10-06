#pragma once
#include <stdint.h>

#include <libIDT.hpp>

// Scancodes for keyboard buttons
#define LeftShift 0x2A
#define RightShift 0x36
#define Left 0x4b
#define Right 0x4d
#define Up 0x48
#define Down 0x50
#define Spacebar 0x39
#define Enter 0x1C
#define BackSpace 0x0E

// ASCII codes for special keys
#define Escape_ASCII 0x1b
#define BackSpace_ASCII 0x08
#define Enter_ASCII '\n'

namespace KB {
uint8_t getKeyBufferIndex();
void pushKeyBuffer(char chr);
char popKeyBuffer();

void init(void);
char Translate(uint8_t scancode, bool uppercase);

void waitForKeyboard();
}  // namespace KB

INTERRUPT void keyboardHandler(interrupt_frame* frame);