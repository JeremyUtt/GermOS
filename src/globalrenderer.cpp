#include "converts.h"
#include "globalrenderer.h"
#include <parameters.h>
#include <fonts.h>
#include <utils.h>
#include <stdint.h>
#include <serial.h>



// unsigned int font[95][13] = {
// 	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},// space :32
// 	{0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},// ! :33
// 	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36},
// 	{0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00},
// 	{0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18},
// 	{0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70},
// 	{0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38},
// 	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e},
// 	{0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c},
// 	{0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30},
// 	{0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00},
// 	{0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00},
// 	{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03},
// 	{0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c},
// 	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18},
// 	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e},
// 	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e},
// 	{0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c},
// 	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
// 	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
// 	{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff},
// 	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
// 	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
// 	{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06},
// 	{0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60},
// 	{0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e},
// 	{0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
// 	{0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
// 	{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
// 	{0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc},
// 	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
// 	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff},
// 	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
// 	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
// 	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
// 	{0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
// 	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3},
// 	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
// 	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},
// 	{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},
// 	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e},
// 	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
// 	{0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c},
// 	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
// 	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},
// 	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},
// 	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
// 	{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
// 	{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
// 	{0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
// 	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
// 	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff},
// 	{0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c},
// 	{0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60},
// 	{0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c},
// 	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18},
// 	{0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70},
// 	{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
// 	{0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03},
// 	{0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e},
// 	{0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0},
// 	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00},
// 	{0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00},
// 	{0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0},
// 	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78},
// 	{0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00},
// 	{0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00},
// 	{0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x00},
// 	{0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
// 	{0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00},
// 	{0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f},
// 	{0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
// 	{0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0},
// 	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00}  // :126
// };
unsigned int font[95][13] = {};




// struct Font{
// 	int width;
// 	int height;
// 	unsigned int* bitmap;
// 	void setData(int _width, int _height, unsigned int* _bitmap){
// 		width = _width;
// 		height = _height;
// 		bitmap = _bitmap;
// 	};
// 	bool getPixel(int chr, int x, int y){
// 		//chr-=32;
// 		int bit = (bitmap[chr] >> (height*width-(y*width + x))-1) & 1;
// 		if (bit >0){
// 			return 1;
// 		}
// 		return 0;
// 	}
// };


namespace GuiRenderer{

	int Xcounter = 0;
	int Ycounter = 0;
	int printColor;
	Font* activeFont;

	void setDrawColor(int color){
		printColor = color;
	}

	void setDrawFont(Font font){
		activeFont = &font;
	}

	void UpdateCounter(int xInc, int yInc){
		int Xmax = screenWidth / (*activeFont).width;
		int Ymax = screenHeight / (*activeFont).height;
		if (xInc < 0) {
			for (int i = 0; i > xInc ; i--){
				if(Xcounter == 0){
					if(Ycounter != 0){
						Xcounter = Xmax;
						Ycounter--;	
					}
					
				}else{
					Xcounter--;
				}			
			}
			
		} 
		else {
			for (int i = 0; i < xInc ; i++){
				if(Xcounter == Xmax){
					Xcounter = 0;
					Ycounter++;
				}else{
					Xcounter++;
				}			
			}
		}
		Ycounter+=yInc;
	}

	void ClearScreen(){
		for (int i = 0; i < screenHeight; i++){
			for (int j = 0; j < screenWidth; j++){
				putPixel(j, i, 0);
			}
		}
		Xcounter = 0;
		Ycounter = 0;
	}
//-----
	void putPixel(int pos_x, int pos_y, unsigned char VGA_COLOR){
		unsigned char* location = (unsigned char*)screenMemory + screenWidth * pos_y + pos_x;
		*location = VGA_COLOR;
	}

	void putRect(int x, int y, int width, int height, int color){
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				putPixel(x+j, y+i, color);
			}
		}
	}

	void putLine(int x, int y, int lenght, bool vertical, int color){
		if (vertical){	
			for (int i = 0; i < lenght; i++){
				putPixel(x, y+i, color);
			}
		}else{
			for (int i = 0; i < lenght; i++){
				putPixel(x+i, y, color);
			}
		}
		
		
		
		
		
		
	}
//-----
	void putChar(int chr, int x, int y){
		for (int row = 0; row < (*activeFont).height; row++){
			for (int pixel = 0; pixel < (*activeFont).width; pixel++){
				if ((*activeFont).getPixel(chr, pixel, row)){
					putPixel(x+pixel, y+row, printColor);
				}	
			}
		}
	}	

	void putString(char* string, int x, int y){
		for (int character = 0; character < getStrLen(string); character++){
			putChar(string[character], x+(character*((*activeFont).width +1)), y);
		}
		
	}
//-----
	void println(const char String[]){
		putString((char*)String, Xcounter, Ycounter*((*activeFont).height+1));
		UpdateCounter(getStrLen((char*)String), 0);
		Xcounter = 0;
		Ycounter++;
	}

	void printText(const char String[]){
		putString((char*)String, Xcounter, Ycounter*((*activeFont).height+1));
		UpdateCounter(getStrLen((char*)String), 0);
	}

	void printChar(const char chr){
		putChar(chr, Xcounter*((*activeFont).width + 1), Ycounter*((*activeFont).height+1));
		UpdateCounter(1, 0);
	}
}


namespace NewGuiRenderer{
	static int Xcounter = 0;
	static int Ycounter = 0;
	static int printColor;
	static NewFont activeFont;

	void setDrawColor(int color){
		printColor = color;
	}

	void setDrawFont(NewFont* font){
		activeFont = *font;
	}

	void UpdateCounter(int xInc, int yInc){
		int Xmax = screenWidth / activeFont.width;
		int Ymax = screenHeight / activeFont.height;
		if (xInc < 0) {
			for (int i = 0; i > xInc ; i--){
				if(Xcounter == 0){
					if(Ycounter != 0){
						Xcounter = Xmax;
						Ycounter--;	
					}
					
				}else{
					Xcounter--;
				}			
			}
			
		} 
		else {
			for (int i = 0; i < xInc ; i++){
				if(Xcounter == Xmax){
					Xcounter = 0;
					Ycounter++;
				}else{
					Xcounter++;
				}			
			}
		}
		Ycounter+=yInc;
	}

	void ClearScreen(){
		for (int i = 0; i < screenHeight; i++){
			for (int j = 0; j < screenWidth; j++){
				putPixel(j, i, 0);
			}
		}
		Xcounter = 0;
		Ycounter = 0;
	}
//-----
	void putPixel(int pos_x, int pos_y, unsigned char VGA_COLOR){
		unsigned char* location = (unsigned char*)screenMemory + screenWidth * pos_y + pos_x;
		*location = VGA_COLOR;
	}

	void putRect(int x, int y, int width, int height, int color){
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				putPixel(x+j, y+i, color);
			}
		}
	}

	void putLine(int x, int y, int lenght, bool vertical, int color){
		if (vertical){	
			for (int i = 0; i < lenght; i++){
				putPixel(x, y+i, color);
			}
		}else{
			for (int i = 0; i < lenght; i++){
				putPixel(x+i, y, color);
			}
		}
		
		
		
		
		
		
	}
//-----
	void putChar(int chr, int x, int y) {
		unsigned char* chrBitmap = activeFont.bitmap[chr];
		
		// serialWriteStr((char*)activeFont.bitmap[chr] +33);
		// 

		int set;
		for (int layer=0; layer < 8; layer++) {
			for (int pixel=0; pixel < 8; pixel++) {
				set = font8x8_basic[chr][layer];
				if (set==0){
					putPixel(x+layer, y+pixel, 14);
				} else {
					putPixel(x+layer, y+pixel, 13);
				}
			}
		}

		// int set;
		// for (int i=0; i < activeFont.height; i++) {
		// 	for (int j=0; j < activeFont.width; j++) {
		// 		set = chrBitmap[i] & 1 << j;
		// 		if (set==0){
		// 			putPixel(x+i, y+j, 14);
		// 		} else {
		// 			putPixel(x+i, y+j, 13);
		// 		}
		// 	}
		// }



	}

	void putString(char* string, int x, int y){
		for (int character = 0; character < getStrLen(string); character++){
			putChar(string[character], x+(character*(activeFont.width +1)), y);
		}
		
	}
//-----
	void println(const char String[]){
		putString((char*)String, Xcounter, Ycounter*(activeFont.height+1));
		UpdateCounter(getStrLen((char*)String), 0);
		Xcounter = 0;
		Ycounter++;
	}

	void printText(const char String[]){
		putString((char*)String, Xcounter, Ycounter*(activeFont.height+1));
		UpdateCounter(getStrLen((char*)String), 0);
	}

	void printChar(const char chr){
		putChar(chr, Xcounter*(activeFont.width + 1), Ycounter*(activeFont.height+1));
		UpdateCounter(1, 0);
	}
}


namespace globalrenderer{
	int xResolution = 320;
	int yResolution = 200;
	int Xcounter1 = 0;
	int Ycounter1 = 0;
	int printColor;
	Font printFont;


	void putPixel(int pos_x, int pos_y, unsigned char VGA_COLOR){
		if (pos_x > 320){
			pos_x = 320;
		}	
		unsigned char* location = (unsigned char*)0xA0000 + 320 * pos_y + pos_x;
		*location = VGA_COLOR;
	}
	//======================
	void putChar(int chr, int x, int y, int color){
		chr-=32;
		for (int row = 0; row < 13; row++){
				for (int pixel = 0; pixel < 16; pixel++){
					int value = (font[chr][13-row] >> (8-pixel)) & 1;
					if(value > 0){
						putPixel(pixel+x, row+y, color);
					}
				}
			}
	}

	void putString(char* string, int x, int y, int color){
		for (int character = 0; character < getStrLen(string); character++){
			putChar(string[character], x+(character*9), y, color);
		}
		
	}
	//======================
	void UpdateCounter(int xInc, int yInc){

		if (xInc < 0) {
			for (int i = 0; i > xInc ; i--){
				if(Xcounter1 == 0){
					if(Ycounter1 != 0){
						Xcounter1 = 35;
						Ycounter1--;	
					}
					
				}else{
					Xcounter1--;
				}			
			}
			
		} 
		else {
			for (int i = 0; i < xInc ; i++){
				if(Xcounter1 == 34){
					Xcounter1 = 0;
					Ycounter1++;
				}else{
					Xcounter1++;
				}			
			}
		}
		Ycounter1+=yInc;
	}

	void setDrawColor(int color){
		printColor = color;
	}

	void setDrawFont(Font font){
		printFont = font;
	}

	void println(const char String[]){
		putString((char*)String, Xcounter1*9, Ycounter1*14, printColor);
		Xcounter1 = 0;
		Ycounter1++;
	}

	void printText(const char String[]){
		putString((char*)String, Xcounter1*9, Ycounter1*14, printColor);
		UpdateCounter(getStrLen((char*)String), 0);
	}

	void printChar(char chr){
		putChar(chr, Xcounter1*9, Ycounter1*14, printColor);
		UpdateCounter(1, 0);
	}

	void printChar_Backspace(){
		UpdateCounter(-1, 0);
		for (int y = 0; y < 14; y++){
			for (int x = 0; x < 9; x++){
				putPixel(Xcounter1*9+x, Ycounter1*14+y, 0);
			}
			
		}
		
	}
	//======================
	void putRect(int x, int y, int width, int height, int color){
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				putPixel(x+j, y+i, color);
			}
		}
	}

	void putLine(int x, int y, int lenght, bool vertical, int color){
		if (vertical){	
			for (int i = 0; i < lenght; i++){
				putPixel(x, y+i, color);
			}
		}else{
			for (int i = 0; i < lenght; i++){
				putPixel(x+i, y, color);
			}
		}
		
		
		
		
		
		
	}

	void ClearScreen(){
		for (int i = 0; i < 200; i++){
			for (int j = 0; j < 320; j++){
				putPixel(j, i, 0);
			}
		}
		Xcounter1 = 0;
		Ycounter1 = 0;
	}
}



