#include <converts.hpp>
#include <firstTUI.hpp>
#include <globalrenderer.hpp>
#include <keyboardHandler.hpp>
#include <serial.hpp>

using namespace TextRenderer;
using namespace keyboardBuffer;

void tui() {
    setDrawColor(0x70);
    for (int j = 0; j < screenHeight; j++) {
        for (int i = 0; i < screenWidth; i++) {
            putChar(' ', i, j);
        }
        putChar(186, 0, j);
        putChar(186, screenWidth - 1, j);
    }

    for (int i = 0; i < screenWidth; i++) {
        putChar(205, i, 0);
        putChar(205, i, screenHeight - 1);
    }

    putChar(201, 0, 0);
    putChar(200, 0, screenHeight - 1);
    putChar(187, screenWidth - 1, 0);
    putChar(188, screenWidth - 1, screenHeight - 1);

    putString("Welcome To GermOS", 31, 0);

    while (true) {
        asm("hlt");

        uint8_t index1 = getKeyBufferIndex();
        // serialWriteStr(intToStr(index1, 10));

        // serialWriteStr("\r\n======\r\n");

        if (index1 > 0) {
            char value = popKeyBuffer();
            serialWriteChar(value);
            
        }

        // uint8_t index2 = getKeyBufferIndex();
        // serialWriteStr(intToStr(index2, 10));
    }
}