//https://www.asciitable.com/
//https://www.youtube.com/channel/UCuWLGQB4WRBKvW1C26zA2og
#include <stdint.h>
#include <parameters.h>
#include <newidt.h>
#include <keyboardHandler.h>
#include <pci.h>
#include <globalrenderer.h>
#include <fonts.h>
#include <kernel.h>
#include <utils.h>
#include <serial.h>
#include <converts.h>
 

extern "C" void main(){	
	initKernel();

	// TODO:fix new char printing
	// all bits being detected at 0, instead oa value at variable location

	//https://github.com/chrisy/fontem 

	amogus();

	while(1) asm("hlt");
	return;
}

static void initKernel(){
	initOldFonts();

	// initFonts();

	using namespace GuiRenderer;

	println("ABCDEFGHIJKLMNO");
	println("abcdefghijklmno");
	println("1234567890");

    load_idt_entry(0x21, (unsigned long) keyboard_handler_int, 0x08, 0x8e);
	idt_init();
    kb_init();
	findRSDP();
	println("Initalizing Serial Port");
	int good = init_serial();
	serialWriteStr("Hello World!");
	println(intToStr(good, 10));
	println("Kernel Initalization Complete!");
	println("Press any key to continue");
	asm("hlt");
	ClearScreen();

}
