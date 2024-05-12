// https://www.asciitable.com/
// https://www.youtube.com/channel/UCuWLGQB4WRBKvW1C26zA2og

#include <stdint.h>

#include <utils.hpp>
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
#include <memory.hpp>
#include <photo.hpp>
#include <process.hpp>
#include <PROGRAM_PONG.hpp>
#include <PROGRAM_TUI.hpp>
#include <string.hpp>
#include <system.hpp>
#include <tests.hpp>
#include <error.hpp>
#include <printf.hpp>


#ifdef TEXT_MODE 
    using namespace TextRenderer; 
    #define startUI() startTUI()
#else 
    using namespace GuiRenderer;      
    #define startUI() startGUI()
#endif


extern "C" void main() {
    initKernel();
    
    // Run Testing Code Here

    // :D

    // Run the UI
    startUI();


    // Runs once everything else is done
    ClearScreen();
    println("All Programs Finished");
    println("Interrupts Disabled");
    println("Processor Halting");
    disableInterrupts();
    halt();
}


void initKernel() {
    setDrawColor(VGA_LIGHT_GRAY);
    setTextFont(&Uni2Terminus12x6psf);

    initMem();
    println("Successfully Switched to Protected Mode");
    println("Setting up Kernel Stack");
    println("Initializing and Loading Graphics Mode Fonts");
    println("Initializing Dynamic Memory Allocator");
    println("Finding Kernel Memory Mapping");
    printMem();
    char* mem = checkKernelMemory(0x7c00 + KERNEL_SIZE-10, 20, "42069");
    if (mem != nullptr) {
        serialWriteStr("\r\n");
        print("    INFO: Start Addr: 0x7c00, End Addr: 0x");
        print(intToStr((uint32_t)mem, 16));
        println("");

    } else {
        setDrawColor(VGA_BROWN);
        println("WARNING: Couldnt find magic number. Kernel May not be fully loaded");
        setDrawColor(VGA_LIGHT_GRAY);
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
    extern binaryFile goopergimg;

    GOOPImage::draw(&goopergimg, 0, 0);

    setDrawColor(0xf);
    putString("Welcome to GoopOS", 100, 10);
    setDrawColor(0x7);

    sleep(5000);
    while (true) {}

    Process pong("Pong", (uint32_t)PONG::main);
    pong.start();
}
#endif