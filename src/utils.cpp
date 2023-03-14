#include <utils.hpp>
#include <parameters.hpp>
#include <globalrenderer.hpp>
#include <stdint.h>

void amogus(){
	int width = 20;
	int height= 20;
	int startx = 50;
	int starty= 50;
	// NewGuiRenderer::putRect(startx, starty, width*3, height*4, 0xaa);
	// NewGuiRenderer::putRect(startx+width, starty+height, width*2, height, 0x55);
	// NewGuiRenderer::putRect(startx, starty+height*4, width, height, 0xaa);
	// NewGuiRenderer::putRect(startx+width*2, starty+height*4, width, height, 0xaa);
	// NewGuiRenderer::putRect(startx-width, starty+height, width, height*3, 0xaa);
	
	NewGuiRenderer::putRect(startx, starty, width*3, height*4, 0xaa);
	NewGuiRenderer::putRect(startx+width, starty+height, width*2, height, 0x55);
	NewGuiRenderer::putRect(startx, starty+height*4, width, height, 0xaa);
	NewGuiRenderer::putRect(startx+width*2, starty+height*4, width, height, 0xaa);
	NewGuiRenderer::putRect(startx-width, starty+height, width, height*3, 0xaa);
}

// static void printColorPallet(){
// 	int color1 = 0;
// 	for (int i = 0; i < screenWidth/20; i++){
// 		for (int j = 0; j < screenHeight/20; j++){
// 			globalrenderer::putRect(i*20, 0, 20, 20, i*j+i);	
// 			color1++;
// 		}	
// 	}
// }

// static void setBackground(int color){
// 	for (int i = 0; i < (80*25); i++){
// 		int g = {*(int*)(screenMemory+2*i)};
// 		int j = {*(int*)(screenMemory+2*i+1)};
// 		// printChar(g, i, 0, (j | color));
// 	}
	
// }

// static bool checkMem(char str[], int address){
//     int strLen = 0;
// 	for (int i = 0; i < 1; i++){
// 		if (str[strLen] != 0){
// 			i--;
// 			strLen++;
// 		}
// 	}

//     for (int i = 0; i < strLen; i++){
//         if(*(char*)(address+i) != str[i]){
//             return false;
//         }
//     }
//     return true;
// }

// int getMemory(int Address){
// 	uint8_t i = {*(uint8_t*)(Address)};
// 	return i;
// }