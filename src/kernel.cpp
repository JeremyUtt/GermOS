// https://www.asciitable.com/
// https://www.youtube.com/channel/UCuWLGQB4WRBKvW1C26zA2og

#include <stdint.h>

#include <error.hpp>
#include <fonts.hpp>
#include <kernel.hpp>
#include <libACPI.hpp>
#include <libGUI.hpp>
// #include <libGUI_old.hpp>
#include <libIDT.hpp>
#include <libIO.hpp>
#include <libKeyboard.hpp>
#include <libPCI.hpp>
#include <libSerial.hpp>
#include <libTimer.hpp>
#include <memory.hpp>
#include <photo.hpp>
#include <printf.hpp>
#include <process.hpp>
#include <PROGRAM_PONG.hpp>
#include <PROGRAM_TUI.hpp>
#include <string.hpp>
#include <system.hpp>
#include <tests.hpp>
#include <utils.hpp>

#ifdef TEXT_MODE
#define startUI() startTUI()
#define boxHeight 25
#define boxWidth 80
#define boxStartY 3

#else
#define startUI() startGUI()
#define boxHeight 200
#define boxWidth 320
#define boxStartY 0
#endif

extern "C" void main() {
    GenericRenderer renderer(0, boxStartY, boxWidth, boxHeight);
    initKernel(renderer);

    // Run Testing Code Here

    // :D

    // Run the UI
    startUI();

    // Runs once everything else is done
    ClearScreen();
    updateStdout(renderer);
    renderer.clearBox();
    printf("All Programs Finished\n");
    printf("Interrupts Disabled\n");
    printf("Processor Halting\n");
    disableInterrupts();
    halt();
}

void initKernel(GenericRenderer& renderer) {
    updateStdout(renderer);
    renderer.setDrawColor(LIGHT_GRAY);

#ifndef TEXT_MODE
    renderer.setTextFont(&Uni2Terminus12x6psf);
#endif

    initMem();
    printf("Successfully Switched to Protected Mode\n");
    printf("Setting up Kernel Stack\n");
    printf("Initializing and Loading Graphics Mode Fonts\n");
    printf("Initializing Dynamic Memory Allocator\n");
    printf("Finding Kernel Memory Mapping\n");
    printMem();
    char* mem = checkKernelMemory(0x7c00 + KERNEL_SIZE - 10, 20, "42069");
    if (mem != nullptr) {
        printf("    INFO: Start Addr: 0x7c00, End Addr: 0x%s\n", intToStr((uint32_t)mem, 16));
    } else {
        renderer.setDrawColor(BROWN);
        printf("WARNING: Couldnt find magic number. Kernel May not be fully loaded\n");
        renderer.setDrawColor(LIGHT_GRAY);
    }

    printf("Initializing Serial Interface\n");
    initSerial();

    printf("Loading IDT Entry for Timer Handler\n");
    loadIdtEntry(0x20, (uint32_t)timerHandler, 0x08, 0x8e);

    printf("Loading IDT Entry for Keyboard Handler\n");
    loadIdtEntry(0x21, (uint32_t)keyboardHandler, 0x08, 0x8e);

    printf("Initializing IDT");
    idtInit();

    printf("Initializing Timer Interrupt\n");
    Timer::setEnabled(true);
    Timer::setFreq(1000);

    printf("Initializing Keyboard Interrupt\n");
    KB::init();

    // println("Finding RSDP Pointer");
    // findRSDP();
    // decodeRSDP(rsdpPtr);

    // println("Decoding RSDT");
    // decodeRSDT(rsdtPtr);

    printf("Kernel Initialization Complete!\n");
    printf("Welcome To GoopOS!\n");
    printf("Press any key to continue\n");

    KB::waitForKeyboard();
    ClearScreen();
}

char* checkKernelMemory(uint32_t start, uint32_t len, string toFind) {
    for (uint32_t i = start; i < start + len; i++) {
        bool goodSoFar = true;
        for (uint32_t j = 0; j < toFind.size(); j++) {
            char character = *(char*)(i + j);

            if (character != toFind.at(j)) {
                goodSoFar = false;
                break;
            }
        }
        if (goodSoFar) {
            return (char*)i;
        }
    }
    return nullptr;
}

#ifdef TEXT_MODE
void startTUI() {
    Process cmd("cmd", (uint32_t)TUI::main);
    cmd.start();
}
#endif

#ifndef TEXT_MODE
void startGUI() {
    // extern binaryFile goopergimg;
    // GOOPImage::draw(&goopergimg, 0, 0);

    // GuiTextRenderer renderer(0, 0, 320, 200);
    // renderer.setTextFont(&Uni2Terminus12x6psf);
    // renderer.setDrawColor(WHITE);
    // renderer.putString("Welcome to GoopOS", 100, 10);
    // renderer.setDrawColor(LIGHT_GRAY);

    // sleep(5000);

    Process pong("Pong", (uint32_t)PONG::main);
    pong.start();
}
#endif