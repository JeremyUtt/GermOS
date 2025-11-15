// https://www.asciitable.com/
// https://www.youtube.com/channel/UCuWLGQB4WRBKvW1C26zA2og

#include <stdint.h>

#include <classTest.hpp>
#include <error.hpp>
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
#include <libVGA.hpp>
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

extern "C" void main() {
    setUiMode(TEXT);
    TuiTextRenderer temp;
    Renderer& renderer = temp;

    initKernel(renderer);

    // decode();
    // char* addr = checkKernelMemory((uint32_t)rsdp, 60000, "MCFG");
    // if(addr != nullptr){
    //     printf("%c", addr);
    // } else{
    //     printf("value is nullpointer\n");
    // }

    for (uint16_t i = 0; i < 6; i++) {
        for (uint16_t j = 0; j < 6; j++) {
            uint16_t word = pciConfigRead32(i, j, 0, 0);
            // fprintf(Serial, "word is: 0x%x %x\n", word<<16, word & 0xffff);
            fprintf(Serial, "word is: 0x%x \n", word);
        }
    }

    // Run the UI
    startTUI();

    // Runs once everything else is done
    renderer.clearBox();
    updateStdout(renderer);
    renderer.clearBox();
    printf("All Programs Finished\n");
    printf("Interrupts Disabled\n");
    printf("Processor Halting\n");
    disableInterrupts();
    halt();
}

void initKernel(Renderer& renderer) {
    updateStdout(renderer);
    renderer.setDrawColor(LIGHT_GRAY);

    renderer.setTextFont(&Uni2Terminus12x6psf);

    initMem();
    printf("Successfully Switched to Protected Mode\n");
    printf("Setting up Kernel Stack\n");
    printf("Initializing and Loading Graphics Mode Fonts\n");
    printf("Initializing Dynamic Memory Allocator\n");
    printf("Finding Kernel Memory Mapping\n");
    printMem();
    char* mem = checkKernelMemory(0x7c00 + KERNEL_SIZE - 10, 20, "42069");
    if (mem != nullptr) {
        printf("\tINFO: Start Addr: 0x7c00, End Addr: 0x%x\n", (int)mem);
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

    printf("Initializing IDT\n");
    idtInit();

    printf("Initializing PIC Timer\n");
    Timer::setEnabled(true);
    Timer::setFreq(1000);

    printf("Initializing PS2 Keyboard Driver\n");
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
    KB::popKeyBuffer();
    renderer.clearBox();
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

void startTUI() {
    Process cmd("cmd", (uint32_t)TUI::main, TEXT);
    cmd.start();
}
