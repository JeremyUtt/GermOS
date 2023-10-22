#pragma once
#include <stdint.h>

namespace keyboardBuffer {
uint8_t getKeyBufferIndex();
void pushKeyBuffer(char chr);
char popKeyBuffer();
}  // namespace keyboardBuffer

void kbInit(void);
extern "C" void keyboard_handler(void);

namespace QWERTYKeyboard {

#define LeftShift 0x2A
#define RightShift 0x36
#define Enter 0x1C
#define BackSpace 0x0E
#define Escape 0x1b
#define Spacebar 0x39
#define LEFT 0x4b
#define RIGHT 0x4d
#define UP 0x48
#define DOWN 0x50
#define Special 0xe0

extern const char ASCIITable[];
char Translate(uint8_t scancode, bool uppercase);
}  // namespace QWERTYKeyboard
