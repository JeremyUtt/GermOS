#pragma once
#include <fonts.hpp>
#include <string.hpp>
namespace GuiRenderer {
const int screenMemory = 0xa0000;
const int screenWidth = 320;
// const int screenWidth = 640;
const int screenHeight = 200;
// const int screenHeight = 350;
void setTextFont(PSF_font*);
void setDrawColor(int color);
void UpdateCounter(int xInc, int yInc);
void ClearScreen();

void putPixel(int pos_x, int pos_y, unsigned char VGA_COLOR);
void putChar(int chr, int x, int y);
void putString(string str, int x, int y);

void println(string str);
void putRect(int x, int y, int width, int height, int color);
void putLine(int x, int y, int length, bool vertical, int color);
void printChar(const char chr);
}  // namespace GuiRenderer

namespace TextRenderer {
const int screenWidth = 80;
const int screenHeight = 25;
const int screenMemory = 0xb8000;
void setTextFont(PSF_font*);
void setDrawColor(int color);
void ClearScreen();
void moveCursor(int x, int y);
void putChar(int chr, int x, int y);
void putString(string str, int x, int y);
void println(string str);
}  // namespace TextRenderer
