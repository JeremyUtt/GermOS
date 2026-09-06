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
#include <dictionary.hpp>

extern "C" uint8_t createFrame();

// void stackTest2(int one, int two, int three) {
//     int local1 = 16 * 4;
//     int local2 = 16 * 5;
//     int local3 = 16 * 6;

//     createFrame();
// }
// void stackTest(int one, int two, int three) {
//     int local1 = 16;
//     int local2 = 16 * 2;
//     int local3 = 16 * 3;

//     stackTest2(4, 5, 6);
// }

extern "C" void main() {
    setUiMode(TEXT);
    TuiTextRenderer temp;
    Renderer& renderer = temp;

    initKernel(renderer);
    // while (true) {
    //     halt();
    // }

    // stackTest(1, 2, 3);
    pciPrintAllDevices(Serial, 3);
    // pciPrintAllDevicesBrief(Serial);


    // ScrollBox scrollBox(10, 10, 300, 200, temp);

    // string content = "Line 1\nLine 2\nLine 3\nLine 4\nLine 5\nLine 6\nLine 7\nLine 8\nLine 9\nLine 10\nLine 11\nLine 12\nLine 13\nLine 14\nLine 15\nLine 16\nLine 17\nLine 18\nLine 19\nLine 20";
    // scrollBox.addContent(content);

    // scrollBox.render();
    // sleep(2000);
    // scrollBox.scrollDown(5);
    // sleep(2000);
    // scrollBox.scrollUp(3);
    // sleep(2000);

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

extern "C" void trampolineFunction() {
    while (true) {
        fprintf(Serial, "Hello World");
    }
}

void initKernel(Renderer& renderer) {
    updateStdout(renderer);
    renderer.setDrawColor(LIGHT_GRAY);

    renderer.setTextFont(&Uni2Terminus12x6psf);

    initMem();
    printf("Successfully Switched to Protected Mode\n");
    printf("Setting up Kernel Stack\n");
    printf("Initializing and Loading Graphics Mode Fonts\n");
    printf("Initializing Dynamic Memory Allocator\n\t");
    printMem();
    printf("Finding Kernel Memory Mapping\n");
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
