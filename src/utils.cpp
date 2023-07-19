#include <stdint.h>

#include <converts.hpp>
#include <libGUI.hpp>
#include <libSerial.hpp>
#include <utils.hpp>

void amogus() {
    int width = 20;
    int height = 20;
    int startx = 1050;
    int starty = 0;
    // NewGuiRenderer::putRect(startx, starty, width*3, height*4, 0xaa);
    // NewGuiRenderer::putRect(startx+width, starty+height, width*2, height,
    // 0x55); NewGuiRenderer::putRect(startx, starty+height*4, width, height,
    // 0xaa); NewGuiRenderer::putRect(startx+width*2, starty+height*4, width,
    // height, 0xaa); NewGuiRenderer::putRect(startx-width, starty+height,
    // width, height*3, 0xaa);

    GuiRenderer::putRect(startx, starty, width * 3, height * 4, 0xaa);
    GuiRenderer::putRect(startx + width, starty + height, width * 2, height,
                            0x55);
    GuiRenderer::putRect(startx, starty + height * 4, width, height, 0xaa);
    GuiRenderer::putRect(startx + width * 2, starty + height * 4, width,
                            height, 0xaa);
    GuiRenderer::putRect(startx - width, starty + height, width, height * 3,
                            0xaa);
}

void printColorPallet() {
    int size = 15;

    int x = 0;
    int y = 0;
    for (int i = 0; i < 256; i++) {
        GuiRenderer::putRect(x, y, size, size, i);
        GuiRenderer::putString((char*)intToStr(i, 10), x, y);

        x += size;
        if (x >= GuiRenderer::screenWidth - size) {
            y += size;
            x = 0;
        }
    }

    // for (int i = 0; i < screenWidth / size; i++) {
    //   for (int j = 0; j < screenHeight / size; j++) {
    //     NewGuiRenderer::putRect(i * size, j * size, size, size, i * j + i);
    //     NewGuiRenderer::putString((char*)intToStr(i * j + i, 16), i * size,
    //                               j * size);
    //     // serialWriteStr
    //     color1++;
    //   }
    // }
}

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

int getMemory(int Address) {
    uint8_t i = {*(uint8_t*)(Address)};
    return i;
}