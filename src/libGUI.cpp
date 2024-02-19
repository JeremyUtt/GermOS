#include <stdint.h>

#include <converts.hpp>
#include <fonts.hpp>
#include <libGUI.hpp>
#include <libIO.hpp>
#include <libSerial.hpp>
#include <utils.hpp>

namespace GuiRenderer {
static int XcounterPx = 0;
static int YcounterPx = 0;
static int printColor;
static PSF_font* currentFont;

void setTextFont(PSF_font* font) {
    currentFont = font;
}

void setDrawColor(int color) {
    printColor = color;
}

void UpdateCounter(int xIncChars, int yIncChars) {
    // Provide
    YcounterPx += yIncChars * currentFont->height;
    XcounterPx += xIncChars * currentFont->width;

    if (yIncChars) {
        XcounterPx = 0;
    }
    if (XcounterPx >= screenWidth) {
        XcounterPx = 0;
        YcounterPx += currentFont->height;
    }

    if (XcounterPx < 0) {
        XcounterPx = screenWidth - 1;
        YcounterPx -= currentFont->height;
    }

    if (YcounterPx > screenHeight) {
        YcounterPx = screenHeight - currentFont->height;
    }

    if (YcounterPx < 0) {
        YcounterPx = 0;
    }
}

void ClearScreen() {
    uint8_t* where = (uint8_t*)screenMemory;
    int i, j;

    for (i = 0; i < screenHeight; i++) {
        for (j = 0; j < screenWidth; j++) {
            where[j] = 0;
        }
        where += screenWidth;
    }

    XcounterPx = 0;
    YcounterPx = 0;
}
//-----
void putPixel(int pos_x, int pos_y, unsigned char VGA_COLOR) {
    unsigned char* location = (unsigned char*)screenMemory + screenWidth * pos_y + pos_x;
    *location = VGA_COLOR;
}

void putRect(int x, int y, int width, int height, int color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            putPixel(x + j, y + i, color);
        }
    }

    // unsigned char *where = vram;
    // int i, j;

    // for (i = 0; i < w; i++) {
    //     for (j = 0; j < h; j++) {
    //         //putpixel(vram, 64 + j, 64 + i, (r << 16) + (g << 8) + b);
    //         where[j] = r;

    //     }
    //     where+=pitch;
    // }
}

void putLine(int x, int y, int length, bool vertical, int color) {
    if (vertical) {
        for (int i = 0; i < length; i++) {
            putPixel(x, y + i, color);
        }
    } else {
        for (int i = 0; i < length; i++) {
            putPixel(x + i, y, color);
        }
    }
}
//-----
void putChar(int chr, int x, int y) {
    // PSF_font* currentFont = &_binary_fonts_Uni2_Terminus12x6_psf_start;
    int paddedWidth = currentFont->width + (8 - currentFont->width % 8);
    int bytesPerLine = paddedWidth / 8;

    for (uint32_t lineNum = 0; lineNum < currentFont->height; lineNum++) {
        for (int bytesNum = 0; bytesNum < bytesPerLine; bytesNum++) {
            uint8_t lineByte = (currentFont->data[chr][lineNum][bytesNum]);

            for (int bitNum = 8 - 1; bitNum >= 0; bitNum--) {
                uint8_t tmpBool = (lineByte >> bitNum);
                bool bit = (tmpBool & 0x01);
                // serialWriteChar((char*)bit ? '#' : '.');
                int pixelX = x + (bytesNum * 8) - bitNum + currentFont->width + 1;
                putPixel(pixelX, y + lineNum, bit ? printColor : 0x00);
            }
        }
        // serialWriteStr((char*)"\r\n");
    }
}

void putString(string str, int x, int y) {
    for (uint32_t i = 0; i < str.size(); i++) {
        int xPos = x + currentFont->width * i;

        if (xPos + currentFont->width >= screenWidth) {
            return;
        }
        putChar(str.at(i), xPos, y);
        // serialWriteStr((char*)"---------------------------------\r\n");
    }
}

//-----
void println(string str) {
    putString(str, XcounterPx, YcounterPx);
    UpdateCounter(str.size(), 1);
}

void printChar(char chr) {
}
}  // namespace GuiRenderer

namespace TextRenderer {

static int Xcounter = 0;
// TODO: fix hacky default y value
static int Ycounter = 4;
static int printColor;
void setDrawColor(int color) {
    printColor = color;
}

void setTextFont(PSF_font* font) {
    return;
}

void ClearScreen() {
    for (int i = 0; i < screenHeight; i++) {
        for (int j = 0; j < screenWidth; j++) {
            putChar(0, j, i);
        }
    }
    Xcounter = 0;
    Ycounter = 0;
    moveCursor(0, 0);
}

void moveCursor(int x, int y) {
    uint16_t pos = y * screenWidth + x;

    outb(TEXT_CURSOR1, 0x0F);
    outb(TEXT_CURSOR2, (uint8_t)(pos & 0xFF));
    outb(TEXT_CURSOR1, 0x0E);
    outb(TEXT_CURSOR2, (uint8_t)((pos >> 8) & 0xFF));
}

void putChar(int chr, int x, int y) {
    uint16_t data = (printColor << 8) + chr;

    uint16_t* location = (uint16_t*)(screenMemory) + screenWidth * y + x;
    *location = data;
}

void putString(string str, int x, int y) {
    for (uint32_t i = 0; i < str.size(); i++) {
        putChar(str.at(i), x + i, y);
    }
}

void println(string str) {
    putString(str, Xcounter, Ycounter);
    Xcounter += str.size();
    if (Xcounter <= screenWidth) {
        Xcounter -= screenMemory;
        Ycounter++;
    }
    Xcounter = 0;
    moveCursor(Xcounter, Ycounter);
}

}  // namespace TextRenderer
