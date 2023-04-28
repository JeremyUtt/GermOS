#include <fonts.hpp>
#include <stdint.h>
#include <serial.hpp>
#include <converts.hpp>
#include <utils.hpp>

void psf_init()
{
    PSF_font* mainFont = (PSF_font*)&_binary_fonts_Uni2_Terminus12x6_psf_start;
    printHeader(mainFont);
    
    for (int i = 0; i < 512; i++)
    {
        translateGlyph(mainFont, i);
    }
}

void printHeader(PSF_font* mainFont){
    serialWriteStr((char*)"\r\nMagic: ");
    serialWriteStr((char*)intToStr(mainFont->magic, 10));
    serialWriteStr((char*)"\r\nVersion:");
    serialWriteStr((char*)intToStr(mainFont->version, 10));
    serialWriteStr((char*)"\r\nHeader Size: ");
    serialWriteStr((char*)intToStr(mainFont->headersize, 10));
    serialWriteStr((char*)"\r\nFlags: ");
    serialWriteStr((char*)intToStr(mainFont->flags, 10));
    serialWriteStr((char*)"\r\nNumber of Glyphs: ");
    serialWriteStr((char*)intToStr(mainFont->numglyph, 10));
    serialWriteStr((char*)"\r\nBytes Per Glyph: ");
    serialWriteStr((char*)intToStr(mainFont->bytesperglyph, 10));
    serialWriteStr((char*)"\r\nHeigth: ");
    serialWriteStr((char*)intToStr(mainFont->height, 10));
    serialWriteStr((char*)"\r\nWidth: ");
    serialWriteStr((char*)intToStr(mainFont->width, 10));
    serialWriteStr((char*)"\r\n");
}

void translateGlyph(PSF_font *font, int character)
{
    int paddedWidth = font->width + (8 - font->width % 8);

    for (int lineNum = 0; lineNum < font->height; lineNum++)
    {
        for (int ColNum = paddedWidth -1; ColNum >= 0; ColNum--)
        {
            char line = (font->data[character][lineNum]);
            char tmpBool = (line >> ColNum);
            bool bit = (tmpBool & 0x01);
            serialWriteChar(bit ? '#' : '.');
        }
        serialWriteStr((char*)"\r\n");
    }
}