#include "libGUI.hpp"

#include <libIO.hpp>
#include <libTimer.hpp>
#include <memory.hpp>
#include <utils.hpp>

// ===============================================
// ============== Renderer Functions =============
// ===============================================

Renderer::Renderer() {
    color = WHITE;
    bgColor = BLACK;
    boxStartX = 0;
    boxStartY = 0;
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

void Renderer::setBackgroundColor(Color color) {
    this->bgColor = color;
}

char* Renderer::saveState() {
    int size = screenWidth * screenHeight * 2;  // times 2 for text mode: 1 byte for color, 1 byte for character

    char* bufferCache = (char*)malloc(size);

    for (int i = 0; i < size; i++) {
        bufferCache[i] = ((uint8_t*)screenMemory)[i];
    }

    return bufferCache;
}

void Renderer::restoreState(char* state) {
    for (int i = 0; i < screenWidth * screenHeight * 2; i++) {
        ((uint8_t*)screenMemory)[i] = state[i];
    }
    free(state);
}

// ===============================================
// ========= TUI Text Renderer Functions =========
// ===============================================

TuiTextRenderer::TuiTextRenderer() : Renderer() {
    screenMemory = 0xB8000;
    screenWidth = 80;
    screenHeight = 25;
    boxWidth = screenWidth;
    boxHeight = screenHeight;
};

TuiTextRenderer::TuiTextRenderer(int boxStartX, int boxStartY, int boxWidth, int boxHeight) : Renderer(boxStartX, boxStartY, boxWidth, boxHeight) {
    screenMemory = 0xB8000;
    screenWidth = 80;
    screenHeight = 25;
};

void TuiTextRenderer::clearBox() {
    for (uint16_t i = boxStartX; i < boxStartX + boxWidth; i++) {
        for (uint16_t j = boxStartY; j < boxStartY + boxHeight; j++) {
            putChar(' ', i - 1, j - 1);
        }
    }
    cursorX = 0;
    cursorY = 0;
}

pair<int, int> TuiTextRenderer::putChar(int chr, int x, int y) {
    pair<int, int> pos = {x, y};

    switch (chr) {
        case '\n':
            pos.second += 1;
            pos.first = 0;
            return pos;
        case '\t':
            for (int8_t i = 0; i < tabSize; i++) {
                putChar(' ', x + i, y);
            }
            pos.first += tabSize;
            return pos;
        default:
            pos.first++;
            break;
    }

    if (x >= boxWidth) {
        x = 0;
        pos.first = 0;
        y++;
        pos.second = y;
    }
    if (y >= boxHeight) {
        return pos;
    }

    uint8_t fullColor = (bgColor << 4) | color;
    uint16_t data = (fullColor << 8) | chr;

    uint16_t* location = (uint16_t*)(screenMemory) + TuiTextRenderer::screenWidth * (y + boxStartY) + (x + boxStartX);
    *location = data;

    return pos;
}

pair<int, int> TuiTextRenderer::putString(string& str, int x, int y) {
    pair<int, int> pos = {x, y};

    for (uint32_t i = 0; i < str.size(); i++) {
        if (x > boxWidth || y > boxHeight) {
            // continue instead of return because of detection of new line
            continue;
        }

        pos = putChar(str.at(i), x, y);
        x = pos.first;
        y = pos.second;
    }
    return pos;
}

pair<int, int> TuiTextRenderer::putString(string&& str, int x, int y) {
    return putString(str, x, y);
}

void TuiTextRenderer::print(string& str) {
    pair<int, int> update = putString(str, cursorX, cursorY);
    cursorX = update.first;
    cursorY = update.second;
    updateCursor();
}

void TuiTextRenderer::print(string&& str) {
    print(str);
}

void TuiTextRenderer::printChar(char chr) {
    pair<int, int> pos = putChar(chr, cursorX, cursorY);
    cursorX = pos.first;
    cursorY = pos.second;
    updateCursor();
}

void TuiTextRenderer::updateCursor() {
    uint16_t pos = boxStartX + cursorX + (boxStartY + cursorY) * TuiTextRenderer::screenWidth;

    outb(TEXT_CURSOR1, 0x0F);
    outb(TEXT_CURSOR2, (uint8_t)(pos & 0xFF));
    outb(TEXT_CURSOR1, 0x0E);
    outb(TEXT_CURSOR2, (uint8_t)((pos >> 8) & 0xFF));
}

void TuiTextRenderer::backspace() {
    if (cursorX <= 0) {
        cursorX = TuiTextRenderer::screenWidth - 1;
        cursorY--;
    } else {
        cursorX--;
    }
    putChar(' ', cursorX, cursorY);
    updateCursor();
}

void TuiTextRenderer::setTextFont(PSF_font* font) {
    // Do nothing
}

// ===============================================
// ============ GUI Renderer Functions ===========
// ===============================================

GuiTextRenderer::GuiTextRenderer() : Renderer() {
    screenMemory = 0xA0000;
    screenWidth = 320;
    screenHeight = 200;
    boxWidth = screenWidth;
    boxHeight = screenHeight;
};

GuiTextRenderer::GuiTextRenderer(int boxStartX, int boxStartY, int boxWidth, int boxHeight) : Renderer(boxStartX, boxStartY, boxWidth, boxHeight) {
    screenMemory = 0xA0000;
    screenWidth = 320;
    screenHeight = 200;
};

void GuiTextRenderer::setTextFont(PSF_font* font) {
    currentFont = font;
}

pair<int, int> GuiTextRenderer::putChar(int chr, int x, int y) {
    int paddedWidth = currentFont->width + (8 - currentFont->width % 8);
    int bytesPerLine = paddedWidth / 8;

    pair<int, int> pos = {x, y};

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

        pos = putChar(str.at(i), x, y);
        x = pos.first;
        y = pos.second;
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
    putRect(boxStartX, boxStartY, boxWidth, boxHeight, (int)BLACK);
    cursorX = 0;
    cursorY = 0;
}

void GuiTextRenderer::backspace() {
    if (cursorX <= 0) {
        cursorX = boxWidth - currentFont->width;
        cursorY--;
    } else {
        cursorX -= currentFont->width;
    }
    putChar(' ', cursorX, cursorY);
}

void GuiTextRenderer::putRect(int x, int y, int width, int height, uint8_t color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            putPixelM_new(x + j, y + i, color);
        }
    }
}

void GuiTextRenderer::putLine(int x, int y, int length, bool vertical, uint8_t color) {
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

void GuiTextRenderer::putRect(int x, int y, int width, int height, Color color) {
    putRect(x, y, width, height, (uint8_t)color);
}

void GuiTextRenderer::putLine(int x, int y, int length, bool vertical, Color color) {
    putLine(x, y, length, vertical, (uint8_t)color);
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
    pog2.putRect(50, 50, 320 - 100, 200 - 100, (int)BLUE);
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