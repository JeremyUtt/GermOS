#include "libVGA.hpp"

#include <libIO.hpp>

#define GRAPHICS_MODE_REG 0x3CE
#define GRAPHICS_MODE_DATA 0x3CF

void set_mode_13h() {
    outb(0x03C8, 0x00);  // Set the VGA palette

    for (int i = 0; i < 256; i++) {
        outb(0x03C9, i);  // Red
        outb(0x03C9, i);  // Green
        outb(0x03C9, i);  // Blue
    }

    outb(0x03C2, 0x63);  // Miscellaneous Output register

    // Sequencer registers
    outb(0x03C4, 0x00);
    outb(0x03C5, 0x03);
    outb(0x03C4, 0x01);
    outb(0x03C5, 0x01);
    outb(0x03C4, 0x02);
    outb(0x03C5, 0x0F);
    outb(0x03C4, 0x03);
    outb(0x03C5, 0x00);
    outb(0x03C4, 0x04);
    outb(0x03C5, 0x0E);

    // CRT Controller registers
    outb(0x03D4, 0x11);
    outb(0x03D5, 0x0E);
    outb(0x03D4, 0x00);
    outb(0x03D5, 0x5F);
    outb(0x03D4, 0x01);
    outb(0x03D5, 0x1F);
    outb(0x03D4, 0x02);
    outb(0x03D5, 0x1F);
    outb(0x03D4, 0x03);
    outb(0x03D5, 0xA0);
    outb(0x03D4, 0x04);
    outb(0x03D5, 0xBF);
    outb(0x03D4, 0x05);
    outb(0x03D5, 0x0B);
    outb(0x03D4, 0x06);
    outb(0x03D5, 0x1F);
    outb(0x03D4, 0x07);
    outb(0x03D5, 0x1D);
    outb(0x03D4, 0x08);
    outb(0x03D5, 0x00);
    outb(0x03D4, 0x09);
    outb(0x03D5, 0x4F);
    outb(0x03D4, 0x10);
    outb(0x03D5, 0x9C);
    outb(0x03D4, 0x11);
    outb(0x03D5, 0x8E);
    outb(0x03D4, 0x12);
    outb(0x03D5, 0x8F);
    outb(0x03D4, 0x13);
    outb(0x03D5, 0x28);
    outb(0x03D4, 0x14);
    outb(0x03D5, 0x1F);
    outb(0x03D4, 0x15);
    outb(0x03D5, 0x96);
    outb(0x03D4, 0x16);
    outb(0x03D5, 0xB9);
    outb(0x03D4, 0x17);
    outb(0x03D5, 0xA3);

    // Graphics Controller registers
    outb(GRAPHICS_MODE_REG, 0x00);
    outb(GRAPHICS_MODE_DATA, 0x00);
    outb(GRAPHICS_MODE_REG, 0x01);
    outb(GRAPHICS_MODE_DATA, 0x00);
    outb(GRAPHICS_MODE_REG, 0x02);
    outb(GRAPHICS_MODE_DATA, 0x00);
    outb(GRAPHICS_MODE_REG, 0x03);
    outb(GRAPHICS_MODE_DATA, 0x00);
    outb(GRAPHICS_MODE_REG, 0x04);
    outb(GRAPHICS_MODE_DATA, 0x00);
    outb(GRAPHICS_MODE_REG, 0x05);
    outb(GRAPHICS_MODE_DATA, 0x40);
    outb(GRAPHICS_MODE_REG, 0x06);
    outb(GRAPHICS_MODE_DATA, 0x05);
    outb(GRAPHICS_MODE_REG, 0x07);
    outb(GRAPHICS_MODE_DATA, 0x0F);
    outb(GRAPHICS_MODE_REG, 0x08);
    outb(GRAPHICS_MODE_DATA, 0xFF);

    // Set the VGA palette
    outb(0x03C8, 0x00);  // DAC address write mode register

    for (int i = 0; i < 256; i++) {
        outb(0x03C9, i);  // DAC data register (red)
        outb(0x03C9, i);  // DAC data register (green)
        outb(0x03C9, i);  // DAC data register (blue)
    }

}