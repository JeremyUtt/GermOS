#include <converts.hpp>
#include <libGUI.hpp>
#include <libIO.hpp>
#include <libKeyboard.hpp>
#include <libSerial.hpp>
#include <libVGA.hpp>
#include <PROGRAM_PONG.hpp>
#include <PROGRAM_TUI.hpp>
#include <utils.hpp>
using namespace TextRenderer;
void processCommand(char cmd[]);
void programLoop();
void cmdClear();
void cmdHelp();
void cmdShutdown();
void cmdPong();
int x = 2;
int y = 1;
char command[100] = {};
int cmdIndex = 0;

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
        programLoop();
    }
}

void programLoop() {
    while (keyboardBuffer::getKeyBufferIndex() > 0) {
        char character = keyboardBuffer::popKeyBuffer();

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
                if (cmdIndex <= 0)
                    return;
                x--;
                putChar(' ', x, y);
                moveCursor(x, y);
                cmdIndex--;
                command[cmdIndex] = 0;
                return;
            case Escape:
                serialWriteStr("Escape");
                return;
            default:
                break;
        }

        putChar(character, x, y);

        command[cmdIndex] = character;
        cmdIndex++;

        x++;
        if (x >= screenWidth - 1) {
            y++;
            x = 1;
        }

        moveCursor(x, y);
    }
}

void processCommand(char cmd[]) {
    // serialWriteStr("Processing: ");
    // serialWriteStr(cmd);
    y++;
    x = 1;

    if (strcmp(cmd, (char*)"help"))
        cmdHelp();
    else if (strcmp(cmd, (char*)"clear"))
        cmdClear();
    else if (strcmp(cmd, (char*)"exit"))
        cmdShutdown();
    else if (strcmp(cmd, (char*)"pong"))
        cmdPong();
    else
        putString("Unknown Command. Try: help", x, y);

    x = 2;
    y++;
    putChar('>', 1, y);
    moveCursor(x, y);
}

void cmdHelp() {
    x = 1;
    putString("List of the Currently Avalable Commands:", x, y);
    y++;
    putString("   -help: Prints This Page", x, y);
    y++;
    putString("   -clear: Clears the Terminal", x, y);
    y++;
    putString("   -exit: Shuts Down the Machine", x, y);
}
void cmdClear() {
    for (int i = 1; i < screenWidth - 1; i++) {
        for (int j = 1; j < screenHeight - 1; j++) {
            putChar(' ', i, j);
        }
    }
    x = 1;
    y = 0;
}
void cmdShutdown() {
    // QEMU only
    outw(0x604, 0x2000);
}
void cmdPong() {
    x = 1;
    putString("Not Yet Implemented", x, y);
    y++;

    // TODO: switch to graphics Mode
    // set_mode_13h();

    // returnToReal();
    // pong();

    // TODO: switch to text mode
}