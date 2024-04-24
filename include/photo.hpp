#pragma once

typedef unsigned char imagePPM;

extern imagePPM goop32ppm;
extern imagePPM hi32ppm;

void printPhoto(imagePPM* image, int startX, int startY);