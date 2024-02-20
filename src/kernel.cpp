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
#include <PROGRAM_PONG.hpp>
#include <PROGRAM_TUI.hpp>
#include <string.hpp>
#include <utils.hpp>
#include <libTimer.hpp>
#ifdef TEXT_MODE
using namespace TextRenderer;
#endif
#ifndef TEXT_MODE
using namespace GuiRenderer;
#endif

extern "C" void main() {
    // checkKernelMemory(findRSDP());
    // serialWriteStr("Hello WOrld");

    // vgaSwitch();

    initKernel();
    printIdtEntry(0x21);

#ifndef TEXT_MODE
    pong();
#endif
#ifdef TEXT_MODE
    tui();
#endif

    while (1) {
        ClearScreen();
        println("All Programs Finished");
        asm("hlt");
    }
}

void initKernel() {
    setDrawColor(0x7);
    setTextFont(&_binary_fonts_Uni2_Terminus12x6_psf_start);
    println("Successfully Switched to Protected Mode");
    println("Initalizing and Loading Graphics Mode Fonts");

    println("Initalizing Serial Interface");
    initSerial();

    println("Loading IDT Entry for Timer Handler");
    loadIdtEntry(0x20, (uint32_t)timerHandler, 0x08, 0x8e);

    println("Loading IDT Entry for Keyboard Handler");
    loadIdtEntry(0x21, (uint32_t)keyboardHandler, 0x08, 0x8e);
    

    println("Initalizing IDT");
    idtInit();

    println("Initalizing Timer Interrupt");
    setTimerStatus(true);

    println("Initalizing Keyboard Interrupt");
    kbInit();

    println("Finding RSDP Pointer");
    findRSDP();
    decodeRSDP(rsdpPtr);

    println("Decoding RSDT");
    decodeRSDT(rsdtPtr);

    println("Kernel Initalization Complete!");
    println("Welcome To GermOS!");
    println("Press any key to continue");
    
    waitForKeyboard();
    ClearScreen();
}

bool checkKernelMemory(int start) {
    for (int i = start; i < start + 10000; i++) {
        uint8_t* address = (uint8_t*)(i);
        serialWriteChar(*address);
    }
    return true;
}

