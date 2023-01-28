#pragma once
//https://codeplea.com/embedding-files-in-c-programs

void initFonts();
void initOldFonts();
extern unsigned int BasicFont1[127];
struct NewFont{
	int width;
	int height;
	unsigned char bitmap[128][8];
};

void newnewfontstest();

