#include <stdint.h>

#include <converts.hpp>
#include <fonts.hpp>
#include <globalrenderer.hpp>
#include <serial.hpp>
#include <utils.hpp>

void psf_init() {
    // NewGuiRenderer::setTextFont(&_binary_fonts_Uni2_Terminus12x6_psf_start);
    // PSF_font* fontTest =
    // printHeader(mainFont);
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
