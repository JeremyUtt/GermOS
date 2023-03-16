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
	
	while(1) asm("hlt");
	
	// checkKernelMemory();
	// initKernel();
	psf_init();

	// TODO:fix new char printing
	// all bits being detected at 0, instead oa value at variable location

	//https://github.com/chrisy/fontem 
	amogus();
	while(1) asm("hlt");
	return;
}



static void initKernel(){

	using namespace NewGuiRenderer;

	println((char*)"AAAAAAAAHIJKLMNO\n");
	println((char*)"abcdefghijklmno\n");
	println((char*)"1234567890\n");
	println((char*)"!@#$%^&*(){}[]<>?\\/;:''\n");

    load_idt_entry(0x21, (unsigned long) keyboard_handler_int, 0x08, 0x8e);
	idt_init();
    kb_init();
	findRSDP();
	// println("Initalizing Serial Port");
	int good = init_serial();
	serialWriteStr((char*)"Hello World!");
	println(intToStr(good, 10));
	// println("Kernel Initalization Complete!");
	// println("Press any key to continue");
	asm("hlt");
	ClearScreen();

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
