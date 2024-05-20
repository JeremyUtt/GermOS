#ifdef TEXT_MODE

#include <libGUI.hpp>
// #include <libGUI_old.hpp>
#include <printf.hpp>
#include <libIO.hpp>
#include <libKeyboard.hpp>
#include <libSerial.hpp>
#include <libTimer.hpp>
#include <libVGA.hpp>
#include <PROGRAM_PONG.hpp>
#include <PROGRAM_TUI.hpp>
#include <utils.hpp>
// using namespace TextRenderer;

namespace TUI {

void processCommand(char cmd[]);
void programLoop();
void cmdClear();
void cmdHelp();
void cmdShutdown();
void cmdPong();
char command[CMD_SIZE] = {0};
int cmdIndex = 0;
TuiTextRenderer* textBox;

void main() {
    TuiTextRenderer border(0, 0, screenWidthChar, screenHeightChar);
    border.setDrawColor(BLACK);
    border.setBackgroundColor(LIGHT_GRAY);
    
    TuiTextRenderer temp(1, 1, screenWidthChar-2, screenHeightChar-2);
    textBox = &temp;
    textBox->setDrawColor(BLACK);
    textBox->setBackgroundColor(LIGHT_GRAY);
    updateStdout(*textBox);
    
    
    for (int j = 0; j < screenHeightChar; j++) {
        for (int i = 0; i < screenWidthChar; i++) {
            border.putChar(' ', i, j);
        }
        border.putChar(186, 0, j);
        border.putChar(186, screenWidthChar - 1, j);
    }

    for (int i = 0; i < screenWidthChar; i++) {
        border.putChar(205, i, 0);
        border.putChar(205, i, screenHeightChar - 1);
    }

    border.putChar(201, 0, 0);
    border.putChar(200, 0, screenHeightChar - 1);
    border.putChar(187, screenWidthChar - 1, 0);
    border.putChar(188, screenWidthChar - 1, screenHeightChar - 1);

    border.putString("Welcome To GoopOS", 31, 0);

    
    printf("%c", '>');

    while (true) {
        KB::waitForKeyboard();
        programLoop();
    }
}

void programLoop() {
    while (KB::getKeyBufferIndex() > 0) {
        char character = KB::popKeyBuffer();

        switch (character) {
            case Enter:
            printf("\n");
                processCommand(command);
                for (int i = 0; i < CMD_SIZE; i++) {
                    command[i] = 0;
                }
                cmdIndex = 0;
                return;
            case BackSpace:
                if (cmdIndex <= 0)
                    return;
                textBox->backspace();
                cmdIndex--;
                command[cmdIndex] = 0;
                return;
            case Escape:
                serialWriteStr("Escape");
                return;
            default:
                break;
        }

        if (cmdIndex >= CMD_SIZE) {
            return;
        }

        printf("%c", character);

        command[cmdIndex] = character;
        cmdIndex++;

    }
}

void processCommand(char cmd[]) {
    if (strcmp(cmd, (char*)"help"))
        cmdHelp();
    else if (strcmp(cmd, (char*)"clear"))
        cmdClear();
    else if (strcmp(cmd, (char*)"exit"))
        cmdShutdown();
    else if (strcmp(cmd, (char*)"pong"))
        cmdPong();
    else
        printf("Unknown Command. Try: help\n");

    printf(">");
}

void cmdHelp() {
    printf("List of the Currently Available Commands:\n");
    printf("\t-help: Prints This Page\n");
    printf("\t-clear: Clears the Terminal\n");
    printf("\t-exit: Shuts Down the Machine\n");
}
void cmdClear() {
    textBox->clearBox();
}
void cmdShutdown() {
    // QEMU only
    outw(0x604, 0x2000);
}
void cmdPong() {
    textBox->setDrawColor(RED);
    textBox->setBackgroundColor(BLACK);
    printf("ERROR: Not Yet Implemented\n");
    textBox->setBackgroundColor(LIGHT_GRAY);
    textBox->setDrawColor(BLACK);


    // TODO: switch to graphics Mode
    // set_mode_13h();

    // returnToReal();
    // pong();

    // TODO: switch to text mode
}

}  // namespace TUI

#endif