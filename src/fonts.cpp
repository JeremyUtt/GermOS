#include <fonts.hpp>
#include <stdint.h>
#include <serial.hpp>
#include <converts.hpp>
// #include <libpsf.hpp>

uint16_t *unicode;

void psf_init()
{
    PSF_font* mainFont = (PSF_font*)&_binary_fonts_Uni2_Terminus12x6_psf_start;
    serialWriteStr((char*)"Hello\r\n");
    serialWriteStr((char*)intToStr(mainFont->magic, 10));
    serialWriteStr((char*)"Hello\r\n");
}
