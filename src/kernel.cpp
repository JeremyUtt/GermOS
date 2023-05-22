// https://www.asciitable.com/
// https://www.youtube.com/channel/UCuWLGQB4WRBKvW1C26zA2og

#include <stdint.h>

#include <converts.hpp>
#include <fonts.hpp>
#include <globalrenderer.hpp>
#include <kernel.hpp>
#include <keyboardHandler.hpp>
#include <newidt.hpp>
#include <parameters.hpp>
#include <pci.hpp>
#include <serial.hpp>
#include <utils.hpp>

extern "C" void main() {
  initKernel();

  amogus();
//   printColorPallet();
//   NewGuiRenderer::putString("Hello World!", 0, 0);
//   NewGuiRenderer::putString("Poggies", 0, 70);

  NewGuiRenderer::println("Hello GermOS!!");
  NewGuiRenderer::println("Hello GermOSV2!!");
  NewGuiRenderer::println("awevkjfgbsvtunsgzytaeyufvhsruyngvidhn csehrgyv szuicfgyduyghxdnufchzseuighvseu");

      while (1) {
    asm("hlt");
  }
}

void initKernel() {
  initSerial();
  serialWriteStr("Serial Interface Initalized\r\n");

  serialWriteStr("Loading IDT Entry for Keyboard Handler\r\n");
  loadIdtEntry(0x21, (unsigned long)keyboard_handler_int, 0x08, 0x8e);

  serialWriteStr("Initalizing IDT\r\n");
  idtInit();

  serialWriteStr("Initalizing and Loading Text Fonts\r\n");
  psfInit();

  serialWriteStr("Initalizing Keyboard Interrupt\r\n");
  kbInit();

  // serialWriteStr((char*)"Finding RSDP Pointer\r\n");
  // findRSDP();

  serialWriteStr("Kernel Initalization Complete!\r\n\n");
  serialWriteStr("Welcome To GermOS!\r\n");
  serialWriteStr("Press any key to continue\r\n");
  // asm("hlt");
  // ClearScreen();
}

bool checkKernelMemory() {
  for (int i = 0; i < 100000; i++) {
    uint8_t* address = (uint8_t*)(i);
    serialWriteStr(intToStr(*address, 16));
    serialWriteStr(" ");
  }
  return true;
}