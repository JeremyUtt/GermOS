#include "converts.hpp"
#include "globalrenderer.hpp"
#include <parameters.hpp>
#include <fonts.hpp>
#include <utils.hpp>
#include <stdint.h>
#include <serial.hpp>



// #include "font-notosans-10.h"
// #include "font-ubuntumono-10.h"
#include "fontem.h"


namespace NewGuiRenderer{
	static int Xcounter = 0;
	static int Ycounter = 0;
	static int printColor;

	void setDrawColor(int color){
		printColor = color;
	}

	void UpdateCounter(int xInc, int yInc){
		
		Ycounter += yInc;
		Xcounter += xInc;

		if(Xcounter >= screenWidth){
			Xcounter = 0;
			Ycounter += 20;
		}

		if(Xcounter < 0){
			Xcounter = screenWidth -1;
			Ycounter --;
		}		

		if(Ycounter > screenHeight){
			Ycounter = screenHeight;
		}

		if(Ycounter < 0){
			Ycounter = 0;
		}
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
		
		// const glyph** fontGlyphsArr = font_notosans_10.glyphs;
		// const glyph* CharGlyphPtr = fontGlyphsArr[chr-32]; 
		
		// const uint8_t* CharBitMap = CharGlyphPtr->bitmap;

		// const uint16_t totalCols = CharGlyphPtr->cols + CharGlyphPtr->left + CharGlyphPtr->advance;
		// const uint16_t totalRows = CharGlyphPtr->rows + CharGlyphPtr->top;

		// for (int col = 0; col < totalCols; col++){
			
		// 	// Left Buffer
		// 	if (col <= CharGlyphPtr->left){
		// 		putLine(x + col, y, totalRows, true, 15);
		// 		continue;
		// 	}
			
		// 	// Right Buffer
		// 	if (col >= CharGlyphPtr->left + CharGlyphPtr->cols){
		// 		putLine(x + col, y, totalRows, true, 15);
		// 		continue;
		// 	}
			
		
		// 	for (int row = 0; row <= totalRows; row++){
				
		// 		// Top Buffer
		// 		if (row <= CharGlyphPtr->top){
		// 			putPixel(x + col, y + row, 15);
		// 			continue;
		// 		}

		// 		// row var includes buffer space, needs to be removed to get correct bitmap index
		// 		uint8_t pixelIndex = ((row - CharGlyphPtr->top) * CharGlyphPtr->rows) + (col - CharGlyphPtr->left);
				


		// 		// TODO: have print actually print correct pixels
		// 		uint8_t pixelColor = CharBitMap[pixelIndex];
		// 		// if (pixelColor == 0)
		// 		// {
		// 		// 	putPixel(x + col, y + row, 0);
		// 		// } else {

		// 		// }
				
		// 		putPixel(x + col, y + row, pixelColor);
				


		// 	}
			
		
		// }
	}

	void putString(char* string, int x, int y){
// TODO: Fix
		// for (int character = 0; character < getStrLen(string); character++){		
		// 	putChar(string[character], x+(character*(activeFont.width +1)), y);
		// }	
	}
//-----
	void println(char String[]){
		for (int character = 0; character < getStrLen(String); character++){		
			printChar(String[character]);
		}
	}

	void printChar(const char chr){
		// const glyph* CharGlyphPtr = font_notosans_10.glyphs[chr-32];
		
		// if(chr == '\n'){
		// 	// TODO: make dinamic
		// 	Xcounter = 0;
		// 	Ycounter += 20;
		// 	return;
		// }
		
		// putChar(chr, Xcounter, Ycounter);
		// int xAdvance = CharGlyphPtr->cols + CharGlyphPtr->left + CharGlyphPtr->advance;
		// UpdateCounter(xAdvance, 0);
	}
}


// COPY:



// namespace NewGuiRenderer{
// 	static int Xcounter = 0;
// 	static int Ycounter = 0;
// 	static int printColor;
// 	static NewFont activeFont;

// 	void setDrawColor(int color){
// 		printColor = color;
// 	}

// 	void setDrawFont(NewFont* font){
// 		activeFont = *font;
// 	}

// 	void UpdateCounter(int xInc, int yInc){
// 		int Xmax = screenWidth / activeFont.width;
// 		int Ymax = screenHeight / activeFont.height;
// 		if (xInc < 0) {
// 			for (int i = 0; i > xInc ; i--){
// 				if(Xcounter == 0){
// 					if(Ycounter != 0){
// 						Xcounter = Xmax;
// 						Ycounter--;	
// 					}
					
// 				}else{
// 					Xcounter--;
// 				}			
// 			}
			
// 		} 
// 		else {
// 			for (int i = 0; i < xInc ; i++){
// 				if(Xcounter == Xmax){
// 					Xcounter = 0;
// 					Ycounter++;
// 				}else{
// 					Xcounter++;
// 				}			
// 			}
// 		}
// 		Ycounter+=yInc;
// 	}

// 	void ClearScreen(){
// 		for (int i = 0; i < screenHeight; i++){
// 			for (int j = 0; j < screenWidth; j++){
// 				putPixel(j, i, 0);
// 			}
// 		}
// 		Xcounter = 0;
// 		Ycounter = 0;
// 	}
// //-----
// 	void putPixel(int pos_x, int pos_y, unsigned char VGA_COLOR){
// 		unsigned char* location = (unsigned char*)screenMemory + screenWidth * pos_y + pos_x;
// 		*location = VGA_COLOR;
// 	}

// 	void putRect(int x, int y, int width, int height, int color){
// 		for (int i = 0; i < height; i++){
// 			for (int j = 0; j < width; j++){
// 				putPixel(x+j, y+i, color);
// 			}
// 		}
// 	}

// 	void putLine(int x, int y, int lenght, bool vertical, int color){
// 		if (vertical){	
// 			for (int i = 0; i < lenght; i++){
// 				putPixel(x, y+i, color);
// 			}
// 		}else{
// 			for (int i = 0; i < lenght; i++){
// 				putPixel(x+i, y, color);
// 			}
// 		}
		
		
		
		
		
		
// 	}
// //-----
// 	void putChar(int chr, int x, int y) {
// 		unsigned char* chrBitmap = activeFont.bitmap[chr];
		
// 		char* fontName = font_notosans_10.name;
// 		const glyph** fontGlyphsArr = font_notosans_10.glyphs;
// 		const glyph* CharGlyphPtr = fontGlyphsArr[chr-32]; 
		
// 		const uint8_t* CharBitMap = CharGlyphPtr->bitmap;

// 		const uint16_t totalCols = CharGlyphPtr->cols + CharGlyphPtr->left + CharGlyphPtr->advance;
// 		const uint16_t totalRows = CharGlyphPtr->rows + CharGlyphPtr->top;

// 		for (int col = 0; col < totalCols; col++){
			
// 			// Left Buffer
// 			if (col < CharGlyphPtr->left){
// 				// putLine(x + col, y, totalRows, true, 15);
// 				continue;
// 			}
			
// 			// Right Buffer
// 			if (col > CharGlyphPtr->left + CharGlyphPtr->cols){
// 				// putLine(x + col, y, totalRows, true, 15);
// 				continue;
// 			}
			
		
// 			for (int row = 0; row < totalRows; row++){
				
// 				// Top Buffer
// 				if (row <= CharGlyphPtr->top){
// 					// putPixel(x + col, y + row, 15);
// 					continue;
// 				}

// 				// row var includes buffer space, needs to be removed to get correct bitmap index
// 				uint8_t pixelIndex = ((row - CharGlyphPtr->top) * CharGlyphPtr->rows) + (col - CharGlyphPtr->left);
				
// 				uint8_t pixelColor = CharBitMap[pixelIndex];

// 				// if (pixelColor != 0){
// 					// putPixel(x + col, y + row, 14);
// 				// } else{
// 					// putPixel(x + col, y + row, 13);
// 				// }
				
// 				putPixel(x + col, y + row, 10);


// 			}
			
		
// 		}
// 	}

// 	void putString(char* string, int x, int y){
// 		for (int character = 0; character < getStrLen(string); character++){		
// 			putChar(string[character], x+(character*(activeFont.width +1)), y);
// 		}
		
// 	}
// //-----
// 	void println(const char String[]){
// 		putString((char*)String, Xcounter, Ycounter*(activeFont.height+1));
// 		// UpdateCounter(getStrLen((char*)String), 0);
// 		Xcounter = 0;
// 		Ycounter++;
// 	}

// 	void printText(const char String[]){
// 		putString((char*)String, Xcounter, Ycounter*(activeFont.height+1));
// 		UpdateCounter(getStrLen((char*)String), 0);
// 	}

// 	void printChar(const char chr){
// 		putChar(chr, Xcounter*( + 1), Ycounter*(activeFont.height+1));
// 		const glyph* CharGlyphPtr = font_notosans_10.glyphs[chr-32];
// 		UpdateCounter(CharGlyphPtr->rows, 0);
// 	}
// }

