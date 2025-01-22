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
#include <libVGA.hpp>
#include <tests.hpp>
#include <fonts.hpp>
#include <process.hpp>
#include <photo.hpp>

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
    else if (strcmp(cmd, (char*)"vga"))
        cmdVgatest();
    else if (strcmp(cmd, (char*)"goop"))
        cmdGoop();
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
    char bufferCache[80 * 25 * 2];
    for (int i = 0; i < 80 * 25 * 2; i++) {
        bufferCache[i] = ((uint8_t*)TuiTextRenderer::screenMemory)[i];
    }

    Process cmd("cmd", (uint32_t)PONG::main, GRAPHICS);
    cmd.start();

    updateStdout(*textBox);
    for (int i = 0; i < 320 * 200; i++) {
        ((uint8_t*)TuiTextRenderer::screenMemory)[i] = bufferCache[i];
    }
}

void cmdGoop() {
    // char bufferCache[80 * 25 * 2];
    // for (int i = 0; i < 80 * 25 * 2; i++) {
    //     bufferCache[i] = ((uint8_t*)TuiTextRenderer::screenMemory)[i];
    // }

    char* bufferCache = textBox->saveState();

    Process cmd("goop", (uint32_t)GOOPImage::drawGoop, GRAPHICS);
    cmd.start();

    textBox->restoreState(bufferCache);

    // for (int i = 0; i < 320 * 200; i++) {
    //     ((uint8_t*)TuiTextRenderer::screenMemory)[i] = bufferCache[i];
    // }
}


void cmdVgatest(){
    main2(0, nullptr);

    setUiMode(GRAPHICS);
    GuiTextRenderer renderer;
    renderer.clearBox();
    renderer.setDrawColor(RED);
    renderer.setTextFont(&Uni2Terminus12x6psf);
    renderer.print("Hello World");
    amogus();

    KB::waitForKeyboard();
    KB::popKeyBuffer();

    setUiMode(TEXT);
}

}  // namespace TUI
