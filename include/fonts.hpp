#pragma once
#include <stdint.h>
//https://codeplea.com/embedding-files-in-c-programs

#define PSF_FONT_MAGIC 0x864ab572
extern char _binary_fonts_Uni2_Terminus12x6_psf_start;
extern char _binary_fonts_Uni2_Terminus12x6_psf_end;


typedef struct {
    uint32_t magic;         /* magic bytes to identify PSF */
    uint32_t version;       /* zero */
    uint32_t headersize;    /* offset of bitmaps in file, 32 */
    uint32_t flags;         /* 0 if there's no unicode table */
    uint32_t numglyph;      /* number of glyphs */
    uint32_t bytesperglyph; /* size of each glyph */
    uint32_t height;        /* height in pixels */
    uint32_t width;         /* width in pixels */
    uint8_t data[512][12];
} PSF_font;



void psf_init();
bool getPX(PSF_font* mainFont, char character, int x, int y);
void printHeader(PSF_font* mainFont);
void translateGlyph(PSF_font *font, int character);