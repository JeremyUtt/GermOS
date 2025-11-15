#pragma once

typedef unsigned char binaryFile;

namespace PPMImage {
void printPhoto(binaryFile* image, int startX, int startY);
}

namespace GOOPImage {
void draw(binaryFile* image, int startX, int startY);

void drawGoop();

}  // namespace GOOPImage

// extern binaryFile goop32ppm;
// extern binaryFile hi32ppm;
extern binaryFile goobgimg;
