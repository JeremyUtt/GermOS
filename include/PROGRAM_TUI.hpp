#pragma once

namespace TUI {
void main();
void drawBorder(TuiTextRenderer& border);
}  // namespace TUI

#define CMD_SIZE 100

extern "C" void returnToReal();
