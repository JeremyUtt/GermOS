#include "libGUI.hpp"

#include <libGUI_old.hpp>
#include <libIO.hpp>
#include <libTimer.hpp>
#include <utils.hpp>

#define putPixelM_new(x, y, color) \
    *((unsigned char*)screenMemory + screenWidthPx * (y) + (x)) = color;

void putRect(int x, int y, int width, int height, Color color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            putPixelM_new(x + j, y + i, color);
        }
    }
}

void putLine(int x, int y, int length, bool vertical, Color color) {
    if (vertical) {
        for (int i = 0; i < length; i++) {
            putPixelM_new(x, y + i, color);
        }
    } else {
        for (int i = 0; i < length; i++) {
            putPixelM_new(x + i, y, color);
        }
    }
}

void ClearScreen() {
    uint8_t* where = (uint8_t*)screenMemory;
    uint32_t i, j;

    for (i = 0; i < screenHeightPx; i++) {
        for (j = 0; j < screenWidthPx; j++) {
            where[j] = 0;
        }
        where += screenWidthPx;
    }
}

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

        if (str.at(i) == '\t') {
            for (int8_t i = 0; i < tabSize; i++) {
                putChar(' ', x + i, y);
            }
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

pair<int, int> GuiTextRenderer::putChar(int chr, int x, int y) {
    int paddedWidth = currentFont->width + (8 - currentFont->width % 8);
    int bytesPerLine = paddedWidth / 8;

    pair<int, int> pos = {x, y};

    // however, we still need to update the cursor position from here
    switch (chr) {
        case '\n':
            pos.second += currentFont->height;
            pos.first = 0;
            return pos;
        case '\t':
            pos.first += currentFont->width * tabSize;
            for (int i = 0; i < tabSize; i++) {
                putChar(' ', x + i * currentFont->width, y);
            }
            return pos;
        default:
            pos.first += currentFont->width;
            break;
    };

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

    return pos;
}

pair<int, int> GuiTextRenderer::putString(string& str, int x, int y) {
    pair<int, int> pos = {x, y};

    for (uint32_t i = 0; i < str.size(); i++) {
        if (x + currentFont->width > boxWidth || y + currentFont->height > boxHeight) {
            // continue instead of return because of detection of new line
            continue;
        }

        pos = putChar(str.at(i), x + i*currentFont->width, y);
    }
    return pos;
}

pair<int, int> GuiTextRenderer::putString(string&& str, int x, int y) {
    return putString(str, x, y);
}

void GuiTextRenderer::print(string& str) {
    pair<int, int> update = putString(str, cursorX, cursorY);
    cursorX = update.first;
    cursorY = update.second;
}

void GuiTextRenderer::print(string&& str) {
    print(str);
}

void GuiTextRenderer::printChar(char chr) {
    pair<int, int> pos = putChar(chr, cursorX, cursorY);
    cursorX = pos.first;
    cursorY = pos.second;
}

void GuiTextRenderer::clearBox() {
    GuiRenderer::putRect(boxStartX, boxStartY, boxWidth, boxHeight, BLACK);
    cursorX = 0;
    cursorY = 0;
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