#ifdef TEXT_MODE

#pragma once

namespace TUI {
void main();
void drawBorder(TuiTextRenderer &border);
}

#define CMD_SIZE 100

extern "C" void returnToReal();

#endif