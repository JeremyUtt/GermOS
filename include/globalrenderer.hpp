#pragma once
#include <fonts.hpp>

namespace NewGuiRenderer {
void setTextFont(PSF_font*);
void setDrawColor(int color);
void UpdateCounter(int xInc, int yInc);
void ClearScreen();

void putPixel(int pos_x, int pos_y, unsigned char VGA_COLOR);
void putChar(int chr, int x, int y);
void putString(char* string, int x, int y);
void putString(const char* string, int x, int y);

void println(char String[]);
void println(const char String[]);
void putRect(int x, int y, int width, int height, int color);
void putLine(int x, int y, int lenght, bool vertical, int color);
void printChar(const char chr);
}  // namespace NewGuiRenderer

namespace TextRenderer {
void setDrawColor(int color);
void putChar(int chr, int x, int y);
void putString(char* string, int x, int y);
void putString(const char* string, int x, int y);
void println(char String[]);
void println(const char String[]);
}  // namespace TextRenderer
