#pragma once
#include <stdint.h>
// https://codeplea.com/embedding-files-in-c-programs

#define PSF_FONT_MAGIC 0x864ab572

struct PSF_font{
    uint32_t magic;         /* magic bytes to identify PSF */
    uint32_t version;       /* zero */
    uint32_t headerSize;    /* offset of bitmaps in file, 32 */
    uint32_t flags;         /* 0 if there's no unicode table */
    uint32_t numGlyph;      /* number of glyphs */
    uint32_t bytesPerGlyph; /* size of each glyph */
    uint32_t height;        /* height in pixels */
    uint32_t width;         /* width in pixels */
    uint8_t data[512][12][1];
    // uint8_t data[256][20][2];
};

extern PSF_font _binary_fonts_Uni2_Terminus12x6_psf_start;
extern PSF_font _binary_fonts_zap_light20_psf_start;

void psfInit();
bool getPX(PSF_font* mainFont, char character, int x, int y);
void printHeader(PSF_font* mainFont);
void translateGlyph(PSF_font* font, int character);