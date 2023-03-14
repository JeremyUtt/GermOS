#pragma once
#include <fonts.hpp>
struct Font{
	int width;
	int height;
	unsigned int* bitmap;
	void setData(int _width, int _height, unsigned int* _bitmap){
		width = _width;
		height = _height;
		bitmap = _bitmap;
	};
	bool getPixel(int chr, int x, int y){
		//chr-=32;
		int bit = (bitmap[chr] >> (height*width-(y*width + x)-1)) & 1;
		if (bit >0){
			return 1;
		}
		return 0;
	}
};



namespace globalrenderer{
	void putPixel(int pos_x, int pos_y, unsigned char VGA_COLOR);
	void putChar(int chr, int x, int y, int color);
	void putString(char* string, int x, int y, int color);
	void putRect(int x, int y, int width, int height, int color);
	void UpdateCounter(int xInc, int yInc);
	void setDrawColor(int color);
	void println(const char String[]);
	void printText(const char String[]);
	void putLine(int x, int y, int lenght, bool vertical, int color);
	void ClearScreen();
	void printChar(char chr);
	void printChar_Backspace();
}

namespace NewGuiRenderer{
	void setDrawColor(int color);
	void setDrawFont(NewFont* font);
	void UpdateCounter(int xInc, int yInc);
	void ClearScreen();
	void putPixel(int pos_x, int pos_y, unsigned char VGA_COLOR);
	void putChar(int chr, int x, int y);
	void putString(char* string, int x, int y);
	void println(char String[]);
	void putRect(int x, int y, int width, int height, int color);
	void putLine(int x, int y, int lenght, bool vertical, int color);
	void printChar(const char chr);
}