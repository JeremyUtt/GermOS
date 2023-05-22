// https://wiki.osdev.org/Printing_To_Screen
#include "terminal.hpp"

#include "converts.hpp"
#include "io.hpp"
#include "parameters.hpp"

int Ycounter = 0;
int Xcounter = 0;

void update_cursor(int x, int y) {
    uint16_t pos = y * 80 + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void UpdateCounter(int xInc, int yInc) {
    if (xInc < 0) {
        for (int i = 0; i > xInc; i--) {
            if (Xcounter == 0) {
                Xcounter = 79;
                Ycounter--;
            } else {
                Xcounter--;
            }
        }

    } else {
        for (int i = 0; i < xInc; i++) {
            if (Xcounter == 79) {
                Xcounter = 0;
                Ycounter++;
            } else {
                Xcounter++;
            }
        }
    }
    Ycounter += yInc;
    update_cursor(Xcounter, Ycounter);
}

void println_MASTER(char string[], int color) {
    Xcounter = 0;
    Ycounter++;
    int strLen = 0;
    for (int i = 0; i < 1; i++) {
        if (string[strLen] != 0) {
            i--;
            strLen++;
        }
    }

    for (int i = 0; i < strLen; i++) {
        *(char*)((screenMemory + 160 * Ycounter) + 2 * i) = string[i];
        *(char*)((screenMemory + 160 * Ycounter) + 2 * i + 1) = color;
    }
    UpdateCounter(strLen, 0);
}

void println(int number, int color, int base) {
    char* string = intToStr(number, base);
    println_MASTER(string, color);
}

void println(const char string[], int color) {
    println_MASTER((char*)string, color);
}

void println(char string[], int color) {
    println_MASTER(string, color);
}

void println_char(char character, int color) {
    Xcounter = 0;
    Ycounter++;
    *(char*)((screenMemory + 160 * Ycounter)) = character;
    *(char*)((screenMemory + 160 * Ycounter) + 1) = color;
}

void printScr_MASTER(char string[], int color) {
    // get string legdgxfddh
    int strLen = 0;
    for (int i = 0; i < 1; i++) {
        if (string[strLen] != 0) {
            i--;
            strLen++;
        }
    }

    for (int i = 0; i < strLen; i++) {
        *(char*)((screenMemory + 160 * Ycounter) + 2 * i + 2 * Xcounter) =
            string[i];
        *(char*)((screenMemory + 160 * Ycounter) + 2 * i + 1 + 2 * Xcounter) =
            color;
    }
    UpdateCounter(strLen, 0);
}

void printScr(int number, int color) {
    char* string = intToStr(number, 16);
    printScr_MASTER(string, color);
}

void printScr(const char string[], int color) {
    printScr_MASTER((char*)string, color);
}

void printScr(char string[], int color) {
    printScr_MASTER(string, color);
}

void printScr_char(char character, int color) {
    *(char*)((screenMemory + 160 * Ycounter) + 2 * Xcounter) = character;
    *(char*)((screenMemory + 160 * Ycounter) + 1 + 2 * Xcounter) = color;
    UpdateCounter(1, 0);
}

void printScr_Backspace(int times) {
    UpdateCounter(-1, 0);
    *(char*)((screenMemory + 160 * Ycounter) + 2 * Xcounter) = 0;
    *(char*)((screenMemory + 160 * Ycounter) + 1 + 2 * Xcounter) = 0x0f;
}

void clearScreen() {
    for (int i = 0; i < 80 * 25; i++) {
        *(char*)(screenMemory + 2 * i) = 0;
        *(char*)(screenMemory + 2 * i + 1) = 0x0f;
    }
    Xcounter = 0;
    Ycounter = 0;
}
