#include <fonts.hpp>
#include <stdint.h>
#include <serial.hpp>
#include <converts.hpp>

uint16_t *unicode;

void psf_init()
{
    // uint16_t glyph = 0;

    /* cast the address to PSF header struct */
    // PSF_font *font = (PSF_font *)&_binary___fonts_Uni2_Terminus12x6_psf_start;
    // /* is there a unicode table? */

    // if (font->flags)
    // {
    //     unicode = NULL;
    //     return;
    // }

    int size = (int)&_binary___fonts_Uni2_Terminus12x6_psf_end - (int)&_binary___fonts_Uni2_Terminus12x6_psf_start;
    for (int i = (int)&_binary___fonts_Uni2_Terminus12x6_psf_start; i < size; i++)
    {
        serialWriteStr(intToStr(i, 16));
        serialWriteStr("Test");
    }
    



// /* 
//     /* get the offset of the table */
//     char *s = (char *)((unsigned char *)&_binary___fonts_Uni2_Terminus12x6_psf_start + font->headersize + font->numglyph * font->bytesperglyph);
//     /* allocate memory for translation table */
    
//     unicode = calloc(USHRT_MAX, 2);
    
//     while (s > _binary___fonts_Uni2_Terminus12x6_psf_end)
//     {
//         uint16_t uc = (uint16_t)((unsigned char *)s[0]);
//         if (uc == 0xFF)
//         {
//             glyph++;
//             s++;
//             continue;
//         }
//         else if (uc & 128)
//         {
//             /* UTF-8 to unicode */
//             if ((uc & 32) == 0)
//             {
//                 uc = ((s[0] & 0x1F) << 6) + (s[1] & 0x3F);
//                 s++;
//             }
//             else if ((uc & 16) == 0)
//             {
//                 uc = ((((s[0] & 0xF) << 6) + (s[1] & 0x3F)) << 6) + (s[2] & 0x3F);
//                 s += 2;
//             }
//             else if ((uc & 8) == 0)
//             {
//                 uc = ((((((s[0] & 0x7) << 6) + (s[1] & 0x3F)) << 6) + (s[2] & 0x3F)) << 6) + (s[3] & 0x3F);
//                 s += 3;
//             }
//             else
//                 uc = 0;
//         }
//         /* save translation */
//         unicode[uc] = glyph;
//         s++;
//     } 

}
