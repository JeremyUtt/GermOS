//https://www.asciitable.com/
//https://www.youtube.com/channel/UCuWLGQB4WRBKvW1C26zA2og
#include <stdint.h>
#include <parameters.hpp>
#include <newidt.hpp>
#include <keyboardHandler.hpp>
#include <pci.hpp>
#include <globalrenderer.hpp>
#include <fonts.hpp>
#include <kernel.hpp>
#include <utils.hpp>
#include <serial.hpp>
#include <converts.hpp>
 






extern "C" void main(){	
	
	amogus();

	initKernel();
	// checkKernelMemory();
	// initKernel();

	// TODO:fix new char printing
	// all bits being detected at 0, instead oa value at variable location

	//https://github.com/chrisy/fontem 
	while(1) asm("hlt");
	return;
}



static void initKernel(){
	// println("Initalizing Serial Port");
	init_serial();
	serialWriteStr((char*)"Serial Interface Initalized\r\n");


	// using namespace NewGuiRenderer;

	
	serialWriteStr((char*)"Loading IDT Entry for Keyboard Handler\r\n");
    load_idt_entry(0x21, (unsigned long) keyboard_handler_int, 0x08, 0x8e);

	serialWriteStr((char*)"Initalizing IDT\r\n");
	idt_init();
    
	
	serialWriteStr((char*)"Initalizing and Loading Text Fonts (INCOMPLETE)\r\n");
	psf_init();
	




	serialWriteStr((char*)"Initalizing Keyboard Interrupt\r\n");
	kb_init();
	
	
	// serialWriteStr((char*)"Finding RSDP Pointer\r\n");
	// findRSDP();
	
	serialWriteStr((char*)"Kernel Initalization Complete!\r\n\n");
	serialWriteStr((char*)"Welcome To GermOS!\r\n");
	serialWriteStr((char*)"Press any key to continue\r\n");
	asm("hlt");
	// ClearScreen();

}



bool checkKernelMemory(){
	for (int i = 0; i < 100000; i++)
	{
		uint8_t* address = (uint8_t*)(i);
		serialWriteStr(intToStr(*address, 16));
		serialWriteStr(" ");
	}
	return true;
}
