#include "pci.h"
#include "io.h"
#include "terminal.h"
#include "converts.h"
#include "globalrenderer.h"
int pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
 
    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
 
    // Write out the address
    outb(0xCF8, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = ((inb(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}


bool check(char str[], int address){
    for (int i = 0; i < getStrLen(str); i++){
        if(*(char*)(address+i) != str[i]){
            return false;
        }
    }
    return true;
}



void findRSDP(){
    char str[] = "RSD PTR ";
    int start_addr = 0x000E0000;
    int i = 0;
    int finds = 0;
    while (true){
        char g = {*(char*)(start_addr+i)};
        char h = {*(char*)(start_addr+i+1)};
        //printInt(start_addr+i, 16, 0, 0, 0x0f);
        
        
        if(check(str, start_addr+i)){
        //    GuiRenderer::printText("--->RSDP found at: 0x");
        //     GuiRenderer::println(intToStr(*(int*)start_addr+i, 16));
        //     GuiRenderer::printText("--->");
        //     GuiRenderer::printChar(g);
        //     GuiRenderer::printChar(h);
        //     GuiRenderer::printChar({*(char*)(start_addr+i+2)});
        //     GuiRenderer::printChar({*(char*)(start_addr+i+3)});
        //     GuiRenderer::printChar({*(char*)(start_addr+i+4)});
        //     GuiRenderer::printChar({*(char*)(start_addr+i+5)});
        //     GuiRenderer::printChar({*(char*)(start_addr+i+6)});
        //     GuiRenderer::printChar({*(char*)(start_addr+i+7)});
        //     GuiRenderer::println("");
            return;
        }
        
      i++;
    }   
}


