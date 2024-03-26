#pragma once
#include <fonts.hpp>
#include <string.hpp>
#include <stdint-gcc.h>

struct vbe_mode_info_structure {
    uint16_t attributes;   // deprecated, only bit 7 should be of interest to you, and it indicates
                           // the mode supports a linear frame buffer.
    uint8_t window_a;      // deprecated
    uint8_t window_b;      // deprecated
    uint16_t granularity;  // deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;  // deprecated; used to switch banks from protected mode without
                            // returning to real mode
    uint16_t pitch;         // number of bytes per horizontal line
    uint16_t width;         // width in pixels
    uint16_t height;        // height in pixels
    uint8_t w_char;         // unused...
    uint8_t y_char;         // ...
    uint8_t planes;
    uint8_t bpp;    // bits per pixel in this mode
    uint8_t banks;  // deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size;  // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer;  // physical address of the linear frame buffer; write here to draw to the
                           // screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;  // size of memory in the framebuffer but not being displayed on
                                   // the screen
    uint8_t reserved1[206];
} __attribute__((packed));

extern vbe_mode_info_structure* vbeInfo;

namespace GuiRenderer {

const int screenMemory = 0xa0000;
const int screenWidth = 320;
const int screenHeight = 200;

// const int screenWidth = 1280;
// const int screenHeight = 1024;
// const int screenMemory = 4244635648;
// const int screenMemory = 0x3000000;

// const int screenWidth = 640;
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
