// https://www.asciitable.com/
// https://www.youtube.com/channel/UCuWLGQB4WRBKvW1C26zA2og

#include <stdint.h>

#include <converts.hpp>
#include <fonts.hpp>
#include <libGUI.hpp>
#include <kernel.hpp>
#include <libKeyboard.hpp>
#include <libIDT.hpp>
#include <parameters.hpp>
#include <pci.hpp>
#include <libSerial.hpp>
#include <utils.hpp>
#include <PROGRAM_TUI.hpp>

using namespace TextRenderer;

extern "C" void main() {
    initKernel();

    tui();

    // amogus();
    // printColorPallet();
    while (1) asm("hlt");
}

void initKernel() {
    setDrawColor(0x7);
    setTextFont(&_binary_fonts_Uni2_Terminus12x6_psf_start);
    println("Successfully Switched to Protected Mode");
    println("Initalizing and Loading Graphics Mode Fonts");

    initSerial();
    println("Serial Interface Initalized");

    println("Loading IDT Entry for Keyboard Handler");
    loadIdtEntry(0x21, (unsigned long)keyboard_handler_int, 0x08, 0x8e);

    println("Initalizing IDT");
    idtInit();


    println("Initalizing Keyboard Interrupt");
    kbInit();

    // serialWriteStr((char*)"Finding RSDP Pointer\r\n");
    // findRSDP();

    println("Kernel Initalization Complete!");
    println("Welcome To GermOS!");
    println("Press any key to continue");
    asm("hlt");
    ClearScreen();
}

bool checkKernelMemory() {
    for (int i = 0; i < 100000; i++) {
        uint8_t* address = (uint8_t*)(i);
        serialWriteStr(intToStr(*address, 16));
        serialWriteStr(" ");
    }
    return true;
}