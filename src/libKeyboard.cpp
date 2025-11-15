#include <stdint.h>

#include <libIDT.hpp>
#include <libIO.hpp>
#include <libKeyboard.hpp>
#include <libSerial.hpp>
#include <system.hpp>
#include <utils.hpp>

namespace KB {
bool isRightShiftPressed;
bool isLeftShiftPressed;
bool isArrowPressed;
char kbBuffer[256] = {0};
uint8_t keyboardBufferIndex = 0;
const char scancodeToASCII[] = {0,   Escape_ASCII, '1',
                           '2', '3',          '4',
                           '5', '6',          '7',
                           '8', '9',          '0',
                           '-', '=',          BackSpace_ASCII,
                           0,   'q',          'w',
                           'e', 'r',          't',
                           'y', 'u',          'i',
                           'o', 'p',          '[',
                           ']', Enter_ASCII,  0,
                           'a', 's',          'd',
                           'f', 'g',          'h',
                           'j', 'k',          'l',
                           ';', '\'',         '`',
                           0,   '\\',         'z',
                           'x', 'c',          'v',
                           'b', 'n',          'm',
                           ',', '.',          '/',
                           0,   '*',          0,
                           ' '};

// Scancode enum for future use
enum Scancodes {
    SC_KEY_ESC = 0x01,
    SC_KEY_1 = 0x02,
    SC_KEY_2 = 0x03,
    SC_KEY_3 = 0x04,
    SC_KEY_4 = 0x05,
    SC_KEY_5 = 0x06,
    SC_KEY_6 = 0x07,
    SC_KEY_7 = 0x08,
    SC_KEY_8 = 0x09,
    SC_KEY_9 = 0x0A,
    SC_KEY_0 = 0x0B,
    SC_KEY_MINUS = 0x0C,
    SC_KEY_EQUAL = 0x0D,
    SC_KEY_BACKSPACE = 0x0E,
    SC_KEY_TAB = 0x0F,
    SC_KEY_Q = 0x10,
    SC_KEY_W = 0x11,
    SC_KEY_E = 0x12,
    SC_KEY_R = 0x13,
    SC_KEY_T = 0x14,
    SC_KEY_Y = 0x15,
    SC_KEY_U = 0x16,
    SC_KEY_I = 0x17,
    SC_KEY_O = 0x18,
    SC_KEY_P = 0x19,
    SC_KEY_LEFTBRACE = 0x1A,
    SC_KEY_RIGHTBRACE = 0x1B,
    SC_KEY_ENTER = 0x1C,
    SC_KEY_LEFTCTRL = 0x1D,
    SC_KEY_A = 0x1E,
    SC_KEY_S = 0x1F,
    SC_KEY_D = 0x20,
    SC_KEY_F = 0x21,
    SC_KEY_G = 0x22,
    SC_KEY_H = 0x23,
    SC_KEY_J = 0x24,
    SC_KEY_K = 0x25,
    SC_KEY_L = 0x26,
    SC_KEY_SEMICOLON = 0x27,
    SC_KEY_APOSTROPHE = 0x28,
    SC_KEY_GRAVE = 0x29,
    SC_KEY_LEFTSHIFT = 0x2A,
    SC_KEY_BACKSLASH = 0x2B,
    SC_KEY_Z = 0x2C,
    SC_KEY_X = 0x2D,
    SC_KEY_C = 0x2E,
    SC_KEY_V = 0x2F,
    SC_KEY_B = 0x30,
    SC_KEY_N = 0x31,
    SC_KEY_M = 0x32,
    SC_KEY_COMMA = 0x33,
    SC_KEY_DOT = 0x34,
    SC_KEY_SLASH = 0x35,
    SC_KEY_RIGHTSHIFT = 0x36,
    SC_KEY_KPASTERISK = 0x37,
    SC_KEY_LEFTALT = 0x38,
    SC_KEY_SPACE = 0x39,
    SC_KEY_CAPSLOCK = 0x3A,
    SC_KEY_F1 = 0x3B,
    SC_KEY_F2 = 0x3C,
    SC_KEY_F3 = 0x3D,
    SC_KEY_F4 = 0x3E,
    SC_KEY_F5 = 0x3F,
    SC_KEY_F6 = 0x40,
    SC_KEY_F7 = 0x41,
    SC_KEY_F8 = 0x42,
    SC_KEY_F9 = 0x43,
    SC_KEY_F10 = 0x44,
    SC_KEY_NUMLOCK = 0x45,
    SC_KEY_SCROLLLOCK = 0x46,
    SC_KEY_KP7 = 0x47,
    SC_KEY_KP8 = 0x48,
    SC_KEY_KP9 = 0x49,
    SC_KEY_KPMINUS = 0x4A,
    SC_KEY_KP4 = 0x4B,
    SC_KEY_KP5 = 0x4C,
    SC_KEY_KP6 = 0x4D,
    SC_KEY_KPPLUS = 0x4E,
    SC_KEY_KP1 = 0x4F,
    SC_KEY_KP2 = 0x50,
    SC_KEY_KP3 = 0x51,
    SC_KEY_KP0 = 0x52,
    SC_KEY_KPDOT = 0x53,
    SC_KEY_F11 = 0x57,
    SC_KEY_F12 = 0x58
};

const char SymbolTable[] = {0, Escape_ASCII, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+'};

void init() {
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

uint8_t getKeyBufferIndex() {
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

void InterpretKeyboard(int keycode) {
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

    char ascii = Translate(keycode, isLeftShiftPressed | isRightShiftPressed);

    if (ascii != 0) {
        KB::pushKeyBuffer(ascii);
    }
}

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

        return scancodeToASCII[scancode] - 32;
    }

    else
        return scancodeToASCII[scancode];
}

void waitForKeyboard() {
    while (KB::getKeyBufferIndex() == 0) {
        halt();
    }
}

}  // namespace KB

#include <printf.hpp>
void newInterpret(int keycode) {
    if (keycode > 0x80) {
        fprintf(Serial, "Released Scancode: 0x%x, ASCII:%c \n", keycode - 0x80, KB::Translate(keycode - 0x80, false));
    } else {
        fprintf(Serial, "Pressed: 0x%x, ASCII:%c\n", keycode, KB::Translate(keycode, false));
    }
}

INTERRUPT void keyboardHandler(interrupt_frame*) {
    signed int keycode;
    keycode = inb(0x60);

    KB::InterpretKeyboard(keycode);

    // newInterpret(keycode);
    // End Interrupt
    outb(PIC1_COMMAND, PIC_EOI);
}
