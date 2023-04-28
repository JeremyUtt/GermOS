#include "keyboardHandler.hpp"
#include "io.hpp"
#include "terminal.hpp"
#include "newidt.hpp"
#include "globalrenderer.hpp"
#include "serial.hpp"
bool isRightShiftPressed;
bool isLeftShiftPressed;
bool isArrowPressed;

void kb_init(void){
    /* This is a very basic keyboard initialization. The assumption is we have a
     * PS/2 keyboard and it is already in a proper state. This may not be the case
     * on real hardware. We simply enable the keyboard interupt */

    /* Get current master PIC interrupt mask */
    unsigned char curmask_master = inb(PIC1_DATA);

    /* 0xFD is 11111101 - enables only IRQ1 (keyboard) on master pic
       by clearing bit 1. bit is clear for enabled and bit is set for disabled */
    outb(PIC1_DATA, curmask_master & 0xFD);


    //outb(PIC1_DATA, 0b111111101);
}

static void InterpretKeyboard(int keycode){
    //printScr(keycode, 0x0f);
    //printScr("-", 0x0f);
        
    // Switch case for non-character keys:
    switch (keycode){
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
        case Enter:
            //printLn("");
            return;
        case BackSpace:
            //TODO: BACKSPACE
            return;
        case UP:
            NewGuiRenderer::UpdateCounter(0, -2);
        case DOWN:
            NewGuiRenderer::UpdateCounter(1, 1);
        case LEFT:
            NewGuiRenderer::UpdateCounter(-2, 0);
        case RIGHT:
            NewGuiRenderer::UpdateCounter(1, 0);

    }


    char ascii = QWERTYKeyboard::Translate(keycode, isLeftShiftPressed | isRightShiftPressed);

    if (ascii != 0){
        // NewGuiRenderer::printChar(ascii);
        serialWriteChar(ascii);
    }

}

extern "C" void keyboard_handler(void){
    signed int keycode;
    keycode = inb(0x60);

    InterpretKeyboard(keycode);

    //End Interrupt
    outb(PIC1_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

namespace QWERTYKeyboard {

    const char ASCIITable[] = {
         0 ,  0 , '1', '2',
        '3', '4', '5', '6',
        '7', '8', '9', '0',
        '-', '=',  0 ,  0 ,
        'q', 'w', 'e', 'r',
        't', 'y', 'u', 'i',
        'o', 'p', '[', ']',
         0 ,  0 , 'a', 's',
        'd', 'f', 'g', 'h',
        'j', 'k', 'l', ';',
        '\'','`',  0 , '\\',
        'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',',
        '.', '/',  0 , '*',
         0 , ' '
    };

    const char SymbolTable[] = {
         0 ,  0 , '!', '@',
        '#', '$', '%', '^',
        '&', '*', '(', ')',
        '_', '+'
    };

    char Translate(uint8_t scancode, bool uppercase){
        if (scancode > 58) return 0; 
     

        if (uppercase){
            switch (scancode){
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

            if (scancode < 14){
                return SymbolTable[scancode];
            }

            return ASCIITable[scancode] - 32;
        }
  
        else return ASCIITable[scancode];
    }

}