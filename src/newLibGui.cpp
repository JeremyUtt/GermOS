#include "newLibGui.hpp"

#include <libGUI.hpp>
#include <libIO.hpp>
#include <libTimer.hpp>
#include <utils.hpp>

#define putPixelM_new(x, y, color) \
    *((unsigned char*)screenMemory + screenWidthPx * (y) + (x)) = color;

// ===============================================
// ============== Renderer Functions =============
// ===============================================

Renderer::Renderer() {
    color = WHITE;
    boxStartX = 0;
    boxStartY = 0;
    boxWidth = screenWidthPx;
    boxHeight = screenHeightPx;
    cursorX = 0;
    cursorY = 0;
}

Renderer::Renderer(int boxStartX, int boxStartY, int boxWidth, int boxHeight) {
    color = WHITE;
    this->boxStartX = boxStartX;
    this->boxStartY = boxStartY;
    this->boxWidth = boxWidth;
    this->boxHeight = boxHeight;
    cursorX = 0;
    cursorY = 0;
}

Renderer::~Renderer() {
}

void Renderer::setDrawColor(Color color) {
    this->color = color;
}
// ===============================================
// ========= TUI Text Renderer Functions =========
// ===============================================

void TuiTextRenderer::clearBox() {
    for (uint16_t i = boxStartX; i < boxWidth; i++) {
        for (uint16_t j = boxStartY; j < boxHeight; j++) {
            putChar(' ', i, j);
        }
    }
}

void TuiTextRenderer::putChar(int chr, int x, int y) {
    uint16_t data = (color << 8) + chr;

    uint16_t* location =
        (uint16_t*)(screenMemory) + screenWidthChar * (y + boxStartY) + (x + boxStartX);
    *location = data;
}

pair<int, int> TuiTextRenderer::putString(string str, int x, int y) {
    pair<int, int> pos = {cursorX, cursorY};

    for (uint32_t i = 0; i < str.size(); i++) {
        if (str.at(i) == '\n') {
            y += 1;
            pos.second += 1;
            x = 0;
            pos.first = 0;
            continue;
        }

        if (x > boxWidth || y > boxHeight) {
            continue;
        }

        putChar(str.at(i), x, y);
        x += 1;
        pos.first += 1;
    }
    return pos;
}
void TuiTextRenderer::print(string str) {
    pair<int, int> update = putString(str, cursorX, cursorY);
    cursorX = update.first;
    cursorY = update.second;
    updateCursor();
}

void TuiTextRenderer::printChar(char chr) {
    putChar(chr, cursorX, cursorY);
    cursorX += 1;
    updateCursor();
}

void TuiTextRenderer::updateCursor() {
    uint16_t pos = boxStartX + cursorX + (boxStartY + cursorY) * screenWidthChar;

    outb(TEXT_CURSOR1, 0x0F);
    outb(TEXT_CURSOR2, (uint8_t)(pos & 0xFF));
    outb(TEXT_CURSOR1, 0x0E);
    outb(TEXT_CURSOR2, (uint8_t)((pos >> 8) & 0xFF));
}

// ===============================================
// ============ GUI Renderer Functions ===========
// ===============================================

void GuiTextRenderer::setTextFont(PSF_font* font) {
    currentFont = font;
}

void GuiTextRenderer::putChar(int chr, int x, int y) {
    int paddedWidth = currentFont->width + (8 - currentFont->width % 8);
    int bytesPerLine = paddedWidth / 8;

    for (uint32_t lineNum = 0; lineNum < currentFont->height; lineNum++) {
        for (int bytesNum = 0; bytesNum < bytesPerLine; bytesNum++) {
            uint8_t lineByte = (currentFont->data[chr][lineNum][bytesNum]);

            for (int bitNum = 8 - 1; bitNum >= 0; bitNum--) {
                uint8_t tmpBool = (lineByte >> bitNum);
                bool bit = (tmpBool & 0x01);
                int pixelX = x + (bytesNum * 8) - bitNum + currentFont->width + 1;
                putPixelM_new(pixelX + boxStartX, y + lineNum + boxStartY, bit ? color : 0x00);
            }
        }
    }
}

pair<int, int> GuiTextRenderer::putString(string str, int x, int y) {
    pair<int, int> pos = {cursorX, cursorY};

    for (uint32_t i = 0; i < str.size(); i++) {
        if (str.at(i) == '\n') {
            y += currentFont->height;
            pos.second += currentFont->height;
            x = 0;
            pos.first = 0;
            continue;
        }

        if (x + currentFont->width > boxWidth || y + currentFont->height > boxHeight) {
            // continue instead of return because of detection of new line
            continue;
        }

        putChar(str.at(i), x, y);
        x += currentFont->width;
        pos.first += currentFont->width;
    }
    return pos;
}

void GuiTextRenderer::print(string str) {
    pair<int, int> update = putString(str, cursorX, cursorY);
    cursorX = update.first;
    cursorY = update.second;
}

void GuiTextRenderer::printChar(char chr) {
    putChar(chr, cursorX, cursorY);
    cursorX += currentFont->width;
}

void GuiTextRenderer::clearBox() {
    GuiRenderer::putRect(boxStartX, boxStartY, boxWidth, boxHeight, BLACK);
}

void newGuiTest() {
    GuiTextRenderer pog(0, 0, 320, 200);

    pog.setTextFont(&Uni2Terminus12x6psf);
    pog.setDrawColor(LIGHT_GRAY);
    pog.print("Hello World!\n");
    pog.print("Hello World!");
    pog.print("Hello World!");
    pog.print("Hello World!\n");
    pog.print("Hello World!");
    pog.print("Hello World!");
    pog.print("Hello World!");
    pog.print("Hello World!\n");

    sleep(1000);
    pog.clearBox();
    sleep(1000);

    GuiTextRenderer pog2(50, 50, 320 - 100, 200 - 100);
    GuiRenderer::putRect(50, 50, 320 - 100, 200 - 100, BLUE);
    pog2.setTextFont(&Uni2Terminus12x6psf);
    pog2.setDrawColor(LIGHT_GRAY);
    pog2.print("Hello World!\n");
    pog2.print("Hello World!");
    pog2.print("Hello World!");
    pog2.print("Hello World!\n");
    pog2.print("Hello World!");
    pog2.print("Hello World!");
    pog2.print("Hello World!");
    pog2.print("Hello World!\n");

    sleep(10000);
}

void newTuiTest() {
    TuiTextRenderer pog(0, 0, 80, 25);

    // pog.setTextFont(&Uni2Terminus12x6psf);
    pog.setDrawColor(LIGHT_GRAY);
    pog.print("Hello World!\n");
    pog.print("Hello World!");
    pog.print("Hello World!");
    pog.print("Hello World!\n");
    pog.print("Hello World!");
    pog.print("Hello World!");
    pog.print("Hello World!");
    pog.print("Hello World!\n");

    sleep(1000);
    pog.clearBox();
    sleep(1000);

    TuiTextRenderer pog2(10, 10, 80 - 20, 25 - 20);
    // GuiRenderer::putRect(50, 50, 320-100, 200-100, BLUE);
    // pog2.setTextFont(&Uni2Terminus12x6psf);
    pog2.setDrawColor(LIGHT_GRAY);
    pog2.print("Hello World!\n");
    pog2.print("Hello World!");
    pog2.print("Hello World!");
    pog2.print("Hello World!\n");
    pog2.print("Hello World!");
    pog2.print("Hello World!");
    pog2.print("Hello World!");
    pog2.print("Hello World!\n");

    sleep(10000);
}