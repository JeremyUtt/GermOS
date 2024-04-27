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
    // GuiRenderer::putRect(50, 50, 50, 50, 0x05);

    initKernel();
    serialWriteStr("Hello World");

    GuiRenderer::putRect(50, 50, 50, 50, 0x0f);

#ifndef TEXT_MODE
    
    extern binaryFile goopergimg;

    GOOPImage::draw(&goopergimg, 0, 0);


    setDrawColor(0xf);
    putString("Welcome to GoopOS", 100, 10);
    setDrawColor(0x7);

    sleep(5000);
    while (true) {}

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
    setTextFont(&Uni2Terminus12x6psf);

    println("Successfully Switched to Protected Mode");
    println("Setting up Kernel Stack");
    println("Initializing and Loading Graphics Mode Fonts");

    println("Finding Kernel Memory Mapping");
    char* mem = checkKernelMemory(0xc000, 0x10000, "42069");
    if(mem != nullptr){
        serialWriteStr("\r\n");
        print("    INFO: Start Addr: 0x7c00, End Addr: 0x");
        print(intToStr((uint32_t)mem, 16));
        println("");

    } else{
        setDrawColor(0x6);
        println("WARNING: Couldnt find magic number. Kernel May not be fully loaded");
        setDrawColor(0x7);
    }

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

char* checkKernelMemory(uint32_t start, uint32_t len, string toFind) {

    for (uint32_t i = start; i < start + len; i++) {

        bool goodSoFar = true;
        for (uint32_t j = 0; j < toFind.size(); j++) {
            char character =  *(char*)(i+j);

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
