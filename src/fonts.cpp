#include <stdint.h>

#include <converts.hpp>
#include <fonts.hpp>
#include <libGUI.hpp>
#include <libSerial.hpp>
#include <utils.hpp>

void psfInit() {
    // NewGuiRenderer::setDrawColor(0x55);

    // NewGuiRenderer::setTextFont(&_binary_fonts_zap_light20_psf_start);
}

void printHeader(PSF_font *mainFont) {
    serialWriteStr((char *)"\r\nMagic: ");
    serialWriteStr((char *)intToStr(mainFont->magic, 10));
    serialWriteStr((char *)"\r\nVersion:");
    serialWriteStr((char *)intToStr(mainFont->version, 10));
    serialWriteStr((char *)"\r\nHeader Size: ");
    serialWriteStr((char *)intToStr(mainFont->headersize, 10));
    serialWriteStr((char *)"\r\nFlags: ");
    serialWriteStr((char *)intToStr(mainFont->flags, 10));
    serialWriteStr((char *)"\r\nNumber of Glyphs: ");
    serialWriteStr((char *)intToStr(mainFont->numglyph, 10));
    serialWriteStr((char *)"\r\nBytes Per Glyph: ");
    serialWriteStr((char *)intToStr(mainFont->bytesperglyph, 10));
    serialWriteStr((char *)"\r\nHeigth: ");
    serialWriteStr((char *)intToStr(mainFont->height, 10));
    serialWriteStr((char *)"\r\nWidth: ");
    serialWriteStr((char *)intToStr(mainFont->width, 10));
    serialWriteStr((char *)"\r\n");
}
