#include <libGUI.hpp>
// #include <libGUI_old.hpp>
#include <libIO.hpp>
#include <libKeyboard.hpp>
#include <libSerial.hpp>
#include <libTimer.hpp>
#include <libVGA.hpp>
#include <printf.hpp>
#include <PROGRAM_PONG.hpp>
#include <PROGRAM_TUI.hpp>
#include <utils.hpp>
#include <vgaModes.hpp>
// using namespace TextRenderer;

namespace TUI {

void processCommand(char cmd[]);
void programLoop();
void cmdClear();
void cmdHelp();
void cmdShutdown();
void cmdPong();
void cmdPrint();
void redrawBorder();
char command[CMD_SIZE] = {0};
int cmdIndex = 0;
TuiTextRenderer* textBox;

void main() {
    TuiTextRenderer border(0, 0, TuiTextRenderer::screenWidth, TuiTextRenderer::screenHeight);
    border.setDrawColor(BLACK);
    border.setBackgroundColor(LIGHT_GRAY);

    TuiTextRenderer temp(1, 1, TuiTextRenderer::screenWidth - 2, TuiTextRenderer::screenHeight - 2);
    textBox = &temp;
    textBox->setDrawColor(BLACK);
    textBox->setBackgroundColor(LIGHT_GRAY);
    updateStdout(*textBox);

    for (int j = 0; j < TuiTextRenderer::screenHeight; j++) {
        for (int i = 0; i < TuiTextRenderer::screenWidth; i++) {
            border.putChar(' ', i, j);
        }
        border.putChar(186, 0, j);
        border.putChar(186, TuiTextRenderer::screenWidth - 1, j);
    }

    for (int i = 0; i < TuiTextRenderer::screenWidth; i++) {
        border.putChar(205, i, 0);
        border.putChar(205, i, TuiTextRenderer::screenHeight - 1);
    }

    border.putChar(201, 0, 0);
    border.putChar(200, 0, TuiTextRenderer::screenHeight - 1);
    border.putChar(187, TuiTextRenderer::screenWidth - 1, 0);
    border.putChar(188, TuiTextRenderer::screenWidth - 1, TuiTextRenderer::screenHeight - 1);

    border.putString("Welcome To GoopOS", 31, 0);

    printf("%c", '>');

    while (true) {
        KB::waitForKeyboard();
        programLoop();
    }
}

void redrawBorder() {
    TuiTextRenderer border(0, 0, TuiTextRenderer::screenWidth, TuiTextRenderer::screenHeight);
    border.setDrawColor(BLACK);
    border.setBackgroundColor(LIGHT_GRAY);

    for (int j = 0; j < TuiTextRenderer::screenHeight; j++) {
        border.putChar(186, 0, j);
        border.putChar(186, TuiTextRenderer::screenWidth - 1, j);
    }

    for (int i = 0; i < TuiTextRenderer::screenWidth; i++) {
        border.putChar(205, i, 0);
        border.putChar(205, i, TuiTextRenderer::screenHeight - 1);
    }

    border.putChar(201, 0, 0);
    border.putChar(200, 0, TuiTextRenderer::screenHeight - 1);
    border.putChar(187, TuiTextRenderer::screenWidth - 1, 0);
    border.putChar(188, TuiTextRenderer::screenWidth - 1, TuiTextRenderer::screenHeight - 1);

    border.putString("Welcome To GoopOS", 31, 0);
}

void programLoop() {
    while (KB::getKeyBufferIndex() > 0) {
        char character = KB::popKeyBuffer();

        switch (character) {
            case Enter_ASCII:
                printf("\n");
                processCommand(command);
                for (int i = 0; i < CMD_SIZE; i++) {
                    command[i] = 0;
                }
                cmdIndex = 0;
                // redraw border incase program messed it up
                redrawBorder();
                return;
            case BackSpace_ASCII:
                if (cmdIndex <= 0)
                    return;
                textBox->backspace();
                cmdIndex--;
                command[cmdIndex] = 0;
                return;
            case Escape_ASCII:
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
    else if (strcmp(cmd, (char*)"print"))
        cmdPrint();
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

void cmdPrint() {
    fprintf(Serial, "Hello World!");
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

    main2(0, nullptr);
    // TODO: switch to graphics Mode
    // set_mode_13h();

    // returnToReal();
    // pong();

    // TODO: switch to text mode
}

}  // namespace TUI
