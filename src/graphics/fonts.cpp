#include <stdint.h>

#include <fonts.hpp>
#include <libSerial.hpp>
#include <utils.hpp>

void printHeader(PSF_font* mainFont) {
    serialWriteStr("\r\nMagic: ");
    serialWriteStr(intToStr(mainFont->magic, 10));
    serialWriteStr("\r\nVersion:");
    serialWriteStr(intToStr(mainFont->version, 10));
    serialWriteStr("\r\nHeader Size: ");
    serialWriteStr(intToStr(mainFont->headerSize, 10));
    serialWriteStr("\r\nFlags: ");
    serialWriteStr(intToStr(mainFont->flags, 10));
    serialWriteStr("\r\nNumber of Glyphs: ");
    serialWriteStr(intToStr(mainFont->numGlyph, 10));
    serialWriteStr("\r\nBytes Per Glyph: ");
    serialWriteStr(intToStr(mainFont->bytesPerGlyph, 10));
    serialWriteStr("\r\nHeigth: ");
    serialWriteStr(intToStr(mainFont->height, 10));
    serialWriteStr("\r\nWidth: ");
    serialWriteStr(intToStr(mainFont->width, 10));
    serialWriteStr("\r\n");
}
