#include "globalrenderer.hpp"

#include <stdint.h>

#include <fonts.hpp>
#include <parameters.hpp>
#include <serial.hpp>
#include <utils.hpp>

#include "converts.hpp"

namespace NewGuiRenderer {
static int Xcounter = 0;
static int Ycounter = 0;
static int printColor;
static PSF_font* currentFont;

void setTextFont(PSF_font* font) {
    currentFont = font;
}

void setDrawColor(int color) {
    printColor = color;
}

void UpdateCounter(int xInc, int yInc) {
    Ycounter += yInc;
    Xcounter += xInc;

    if (Xcounter >= screenWidth) {
        Xcounter = 0;
        Ycounter += 20;
    }

    if (Xcounter < 0) {
        Xcounter = screenWidth - 1;
        Ycounter--;
    }

    if (Ycounter > screenHeight) {
        Ycounter = screenHeight;
    }

    if (Ycounter < 0) {
        Ycounter = 0;
    }
}

void ClearScreen() {
    for (int i = 0; i < screenHeight; i++) {
        for (int j = 0; j < screenWidth; j++) {
            putPixel(j, i, 0);
        }
    }
    Xcounter = 0;
    Ycounter = 0;
}
//-----
void putPixel(int pos_x, int pos_y, unsigned char VGA_COLOR) {
    unsigned char* location =
        (unsigned char*)screenMemory + screenWidth * pos_y + pos_x;
    *location = VGA_COLOR;

}

void putRect(int x, int y, int width, int height, int color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            putPixel(x + j, y + i, color);
        }
    }
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
                serialWriteChar((char*)bit ? '#' : '.');
                int pixelX =
                    x + (bytesNum * 8) - bitNum + currentFont->width + 1;
                putPixel(pixelX, y + lineNum, bit ? printColor : 0x00);
            }
        }
        serialWriteStr((char*)"\r\n");
    }
}

void putString(char* string, int x, int y) {
    // PSF_font* currentFont = &_binary_fonts_Uni2_Terminus12x6_psf_start;

    for (int i = 0; i < getStrLen(string); i++) {
        putChar(string[i], x + (currentFont->width * i), y);
        serialWriteStr((char*)"---------------------------------\r\n");
    }
}
//-----
void println(char String[]) {
}

void printChar(const char chr) {
}
}  // namespace NewGuiRenderer