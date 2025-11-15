#include <libGUI.hpp>
// #include <libGUI_old.hpp>
#include <fonts.hpp>
#include <libIO.hpp>
#include <libKeyboard.hpp>
#include <libSerial.hpp>
#include <libTimer.hpp>
#include <libVGA.hpp>
#include <memory.hpp>
#include <photo.hpp>
#include <printf.hpp>
#include <process.hpp>
#include <PROGRAM_PONG.hpp>
#include <PROGRAM_TUI.hpp>
#include <tests.hpp>
#include <utils.hpp>

// using namespace TextRenderer;

namespace TUI {

void processCommand(char cmd[]);
void programLoop();
void cmdClear();
void cmdHelp();
void cmdShutdown();
void cmdPong();
void cmdPrint();
void cmdVgatest();
void cmdGoop();
void redrawBorder();

char command[CMD_SIZE] = {0};
int cmdIndex = 0;
TuiTextRenderer* textBox;

void main() {
    TuiTextRenderer border;
    border.setDrawColor(BLACK);
    border.setBackgroundColor(LIGHT_GRAY);

    TuiTextRenderer temp(1, 1, border.screenWidth - 2, border.screenHeight - 2);
    textBox = &temp;
    textBox->setDrawColor(BLACK);
    textBox->setBackgroundColor(LIGHT_GRAY);
    updateStdout(*textBox);

    for (int j = 0; j < border.screenHeight; j++) {
        for (int i = 0; i < border.screenWidth; i++) {
            border.putChar(' ', i, j);
        }
        border.putChar(186, 0, j);
        border.putChar(186, border.screenWidth - 1, j);
    }

    for (int i = 0; i < border.screenWidth; i++) {
        border.putChar(205, i, 0);
        border.putChar(205, i, border.screenHeight - 1);
    }

    border.putChar(201, 0, 0);
    border.putChar(200, 0, border.screenHeight - 1);
    border.putChar(187, border.screenWidth - 1, 0);
    border.putChar(188, border.screenWidth - 1, border.screenHeight - 1);

    border.putString("Welcome To GoopOS", 31, 0);

    printf("%c", '>');

    while (true) {
        KB::waitForKeyboard();
        programLoop();
    }
}

void redrawBorder() {
    TuiTextRenderer border;
    border.setDrawColor(BLACK);
    border.setBackgroundColor(LIGHT_GRAY);

    for (int j = 0; j < border.screenHeight; j++) {
        border.putChar(186, 0, j);
        border.putChar(186, border.screenWidth - 1, j);
    }

    for (int i = 0; i < border.screenWidth; i++) {
        border.putChar(205, i, 0);
        border.putChar(205, i, border.screenHeight - 1);
    }

    border.putChar(201, 0, 0);
    border.putChar(200, 0, border.screenHeight - 1);
    border.putChar(187, border.screenWidth - 1, 0);
    border.putChar(188, border.screenWidth - 1, border.screenHeight - 1);

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
    else if (strcmp(cmd, (char*)"vga"))
        cmdVgatest();
    else if (strcmp(cmd, (char*)"goop"))
        cmdGoop();
    else if (strcmp(cmd, (char*)"printMem"))
        printMem();
    else
        printf("Unknown Command. Try: help\n");

    printf(">");
}

void cmdHelp() {
    printf("List of the Currently Available Commands:\n");
    printf("\t-pong: Play Pong\n");
    printf("\t-help: Prints This Page\n");
    printf("\t-clear: Clears the Terminal\n");
    printf("\t-print: prints 'hello world' to serial port\n");
    printf("\t-vga: tests\n");
    printf("\t-goop: draw goop\n");
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
    char* bufferCache = textBox->saveState();

    Process cmd("cmd", (uint32_t)PONG::main, GRAPHICS);
    cmd.start();

    textBox->restoreState(bufferCache);
    updateStdout(*textBox);
}

void cmdGoop() {
    char* bufferCache = textBox->saveState();

    Process cmd("goop", (uint32_t)GOOPImage::drawGoop, GRAPHICS);
    cmd.start();

    textBox->restoreState(bufferCache);
}

void cmdVgatest() {
    char* bufferCache = textBox->saveState();

    // main2(0, nullptr);

    setUiMode(GRAPHICS);
    GuiTextRenderer renderer;
    renderer.clearBox();

    printColorPallet();
    renderer.setDrawColor(RED);
    renderer.setTextFont(&Uni2Terminus12x6psf);
    renderer.putString("Press button", 150, 100);

    KB::waitForKeyboard();
    KB::popKeyBuffer();

    renderer.clearBox();
    renderer.print("Hello World");
    amogus();
    renderer.putString("Press button", 180, 50);

    KB::waitForKeyboard();
    KB::popKeyBuffer();

    setUiMode(TEXT);
    textBox->restoreState(bufferCache);

    printf("Done!\n");
}

}  // namespace TUI
