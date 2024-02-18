#include <converts.hpp>
#include <libGUI.hpp>
#include <libIDT.hpp>
#include <libIO.hpp>
#include <libKeyboard.hpp>
#include <libSerial.hpp>
bool isRightShiftPressed;
bool isLeftShiftPressed;
bool isArrowPressed;

void kbInit(void) {
    /* This is a very basic keyboard initialization. The assumption is we have a
     * PS/2 keyboard and it is already in a proper state. This may not be the
     * case on real hardware. We simply enable the keyboard interupt */

    /* Get current master PIC interrupt mask */
    unsigned char curmask_master = inb(PIC1_DATA);

    /* 0xFD is 11111101 - enables only IRQ1 (keyboard) on master pic
       by clearing bit 1. bit is clear for enabled and bit is set for disabled
     */
    outb(PIC1_DATA, curmask_master & 0xFD);

    // outb(PIC1_DATA, 0b111111101);
}

static void InterpretKeyboard(int keycode) {
    // printScr(keycode, 0x0f);
    // printScr("-", 0x0f);

    // Switch case for shift keys:
    switch (keycode) {
        case LeftShift:
            isLeftShiftPressed = true;
            return;
        case (LeftShift + 0x80):
            isLeftShiftPressed = false;
            return;
        case RightShift:
            isRightShiftPressed = true;
            return;
        case RightShift + 0x80:
            isRightShiftPressed = false;
            return;
    }

    char ascii = QWERTYKeyboard::Translate(
        keycode, isLeftShiftPressed | isRightShiftPressed);

    if (ascii != 0) {
        // NewGuiRenderer::printChar(ascii);
        // serialWriteChar(ascii);
        keyboardBuffer::pushKeyBuffer(ascii);
    }
}

namespace keyboardBuffer {
char kbBuffer[256] = {};
static uint8_t keyboardBufferIndex = 0;

uint8_t getKeyBufferIndex() {
    // for (int i = 0; i < 256; i++)
    // {
    //     serialWriteChar(kbBuffer[i]);
    //     serialWriteChar('-');
    // }

    return keyboardBufferIndex;
}

void pushKeyBuffer(char chr) {
    kbBuffer[keyboardBufferIndex] = chr;
    keyboardBufferIndex += 1;
}
char popKeyBuffer() {
    char chr = kbBuffer[keyboardBufferIndex - 1];

    keyboardBufferIndex -= 1;
    return chr;
}
}  // namespace keyboardBuffer

extern "C" void keyboardHandler(void) {
    signed int keycode;
    keycode = inb(0x60);

    InterpretKeyboard(keycode);

    // End Interrupt
    outb(PIC1_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

namespace QWERTYKeyboard {

const char ASCIITable[] = {
    0,   Escape, '1',       '2', '3',   '4', '5', '6',  '7', '8', '9', '0',
    '-', '=',    BackSpace, 0,   'q',   'w', 'e', 'r',  't', 'y', 'u', 'i',
    'o', 'p',    '[',       ']', Enter, 0,   'a', 's',  'd', 'f', 'g', 'h',
    'j', 'k',    'l',       ';', '\'',  '`', 0,   '\\', 'z', 'x', 'c', 'v',
    'b', 'n',    'm',       ',', '.',   '/', 0,   '*',  0,   ' '};

const char SymbolTable[] = {0,   Escape, '!', '@', '#', '$', '%',
                            '^', '&',    '*', '(', ')', '_', '+'};

char Translate(uint8_t scancode, bool uppercase) {
    if (scancode > 58) {
        return 0;
    }

    if (uppercase) {
        switch (scancode) {
            case 26:
                return '{';
            case 27:
                return '}';
            case 39:
                return ':';
            case 40:
                return '"';
            case 41:
                return '~';
            case 43:
                return '|';
            case 51:
                return '<';
            case 52:
                return '>';
            case 53:
                return '?';
        }

        if (scancode < 14) {
            return SymbolTable[scancode];
        }

        return ASCIITable[scancode] - 32;
    }

    else
        return ASCIITable[scancode];
}

}  // namespace QWERTYKeyboard