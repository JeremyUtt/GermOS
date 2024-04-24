// https://www.asciitable.com/
// https://www.youtube.com/channel/UCuWLGQB4WRBKvW1C26zA2og

#include <stdint.h>

#include <converts.hpp>
#include <fonts.hpp>
#include <kernel.hpp>
#include <libACPI.hpp>
#include <libGUI.hpp>
#include <libIDT.hpp>
#include <libIO.hpp>
#include <libKeyboard.hpp>
#include <libPCI.hpp>
#include <libSerial.hpp>
#include <libTimer.hpp>
#include <photo.hpp>
#include <process.hpp>
#include <PROGRAM_PONG.hpp>
#include <PROGRAM_TUI.hpp>
#include <string.hpp>
#include <system.hpp>
#include <utils.hpp>
#ifdef TEXT_MODE
using namespace TextRenderer;
#endif
#ifndef TEXT_MODE
using namespace GuiRenderer;
#endif

extern "C" void main() {
    initKernel();
    serialWriteStr("Hello World");


    #ifndef TEXT_MODE
        for (int i = 0; i < screenWidth / 32; i++) {
            for (int j = 0; j < screenHeight / 32; j++) {
                // printPhoto(&goop32ppm, 32 * i, 32 * j);
            }
        }

        setDrawColor(0xf);
        putString("Welcome to GoopOS", 100, 10);
        setDrawColor(0x7);
    
        sleep(5000);
        // while (true) {}

        Process pong("Pong", (uint32_t)PONG::main);
        pong.start();
    #endif
    #ifdef TEXT_MODE
        Process cmd("cmd", (uint32_t)TUI::main);
        cmd.start();
    #endif


    ClearScreen();
    println("All Programs Finished");
    println("Interrupts Disabled");
    println("Processor Halting");
    disableInterrupts();
    halt();
    
}

void initKernel() {
    setDrawColor(0x7);
    // setTextFont(&Uni2Terminus12x6psf);

    println("Successfully Switched to Protected Mode");
    println("Setting up Kernel Stack");
    println("Initializing and Loading Graphics Mode Fonts");

    println("Initializing Serial Interface");
    initSerial();

    println("Loading IDT Entry for Timer Handler");
    loadIdtEntry(0x20, (uint32_t)timerHandler, 0x08, 0x8e);

    println("Loading IDT Entry for Keyboard Handler");
    loadIdtEntry(0x21, (uint32_t)keyboardHandler, 0x08, 0x8e);

    println("Initializing IDT");
    idtInit();

    println("Initializing Timer Interrupt");
    Timer::setEnabled(true);
    Timer::setFreq(1000);

    println("Initializing Keyboard Interrupt");
    KB::init();

    // println("Finding RSDP Pointer");
    // findRSDP();
    // decodeRSDP(rsdpPtr);

    // println("Decoding RSDT");
    // decodeRSDT(rsdtPtr);

    println("Kernel Initialization Complete!");
    println("Welcome To GoopOS!");
    println("Press any key to continue");

    KB::waitForKeyboard();
    ClearScreen();
}

bool checkKernelMemory(int start) {
    for (int i = start; i < start + 10000; i++) {
        uint8_t* address = (uint8_t*)(i);
        serialWriteChar(*address);
    }
    return true;
}
