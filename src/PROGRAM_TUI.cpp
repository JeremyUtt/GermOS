#include <PROGRAM_TUI.hpp>
#include <converts.hpp>
#include <globalrenderer.hpp>
#include <keyboardHandler.hpp>
#include <serial.hpp>
#include <utils.hpp>

using namespace TextRenderer;
using namespace keyboardBuffer;
int Xcounter = 2;
int Ycounter = 1;

char command[100] = {};
int cmdIndex = 0;

void processCommand(char cmd[]) {
    serialWriteStr("Processing: ");
    serialWriteStr(cmd);
    if (strcmp(cmd, "help")) {
    }

    Xcounter = 2;
    Ycounter++;
    putChar('>', 1, Ycounter);
    moveCursor(Xcounter, Ycounter);
}

void programloop() {
    while (getKeyBufferIndex() > 0) {
        char character = popKeyBuffer();

        switch (character) {
            case Enter:
                // serialWriteStr("Enter");
                processCommand(command);
                for (int i = 0; i < 100; i++) {
                    command[i] = 0;
                }
                cmdIndex = 0;
                return;
            case BackSpace:
                serialWriteStr("Back");
                return;
            case Escape:
                serialWriteStr("Escape");
                return;
            default:
                break;
        }

        putChar(character, Xcounter, Ycounter);

        command[cmdIndex] = character;
        cmdIndex++;

        Xcounter++;
        if (Xcounter >= screenWidth - 1) {
            Ycounter++;
            Xcounter = 1;
        }

        moveCursor(Xcounter, Ycounter);
    }
}

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

    putChar('>', 1, 1);
    moveCursor(2, 1);

    // char command[] = {};

    while (true) {
        asm("hlt");
        programloop();
    }
}
