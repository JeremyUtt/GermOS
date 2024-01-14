// https://www.asciitable.com/
// https://www.youtube.com/channel/UCuWLGQB4WRBKvW1C26zA2og

#include <stdint.h>

#include <PROGRAM_PONG.hpp>
#include <PROGRAM_TUI.hpp>
#include <converts.hpp>
#include <fonts.hpp>
#include <kernel.hpp>
#include <libACPI.hpp>
#include <libGUI.hpp>
#include <libIDT.hpp>
#include <libKeyboard.hpp>
#include <libPCI.hpp>
#include <libSerial.hpp>
#include <string.hpp>
#include <utils.hpp>

using namespace GuiRenderer;

extern "C" void main() {
    // checkKernelMemory(findRSDP());
    // serialWriteStr("Hello WOrld");
    string name(5);
    name = "poggesdrfsdzfgszdf";

    initKernel();
    
    
    // printColorPallet();
    // printColorPallet();
    // printColorPallet();
    // printColorPallet();
    // printColorPallet();
    amogus();


    for (int i = 0; i < 320*200; i++)
    {
        putPixel(i % 320, i / 320, 4);
        int a = getMemory(screenMemory + i);
        serialWriteStr(intToStr(a, 10));
        serialWriteChar(' '); 
    }
    
    // serialWriteStr(name.str());
    
    // pong();
    // tui();

    // amogus();
    // printColorPallet();
    while (1) {
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

    println("Loading IDT Entry for Keyboard Handler");
    loadIdtEntry(0x21, (unsigned long)keyboardHandlerInt, 0x08, 0x8e);

    println("Initalizing IDT");
    idtInit();

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
    asm("hlt");
    ClearScreen();
}

bool checkKernelMemory(int start) {
    for (int i = start; i < start + 10000; i++) {
        uint8_t* address = (uint8_t*)(i);
        serialWriteChar(*address);
    }
    return true;
}
