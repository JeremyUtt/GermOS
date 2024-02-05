#include <image.hpp>
#include <libGUI.hpp>
#include <libSerial.hpp>
#include <converts.hpp>

using namespace GuiRenderer;

void image() {
    for (int i = 0; i < 320 * 200; i++) {
        serialWriteStr(intToStr(_binary_fonts_amogus_raw_start[i], 10));
        serialWriteChar(' ');
        putPixel(i, 0, _binary_fonts_amogus_raw_start[i]);
    }
    while (true)
    {
        asm("hlt");
    }
    
}
