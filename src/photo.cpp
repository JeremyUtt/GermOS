#include "photo.hpp"

#include <converts.hpp>
#include <libGUI.hpp>
#include <string.hpp>
#include <libSerial.hpp>
using namespace GuiRenderer;

void printPhoto(char* image) {
    // Get position of every newline in header
    uint16_t linesStart[5];
    linesStart[0] = 0;
    uint8_t counter = 1;
    for (uint8_t i = 0; i < 255; i++) {
        if (image[i] == '\n') {
            linesStart[counter] = i + 1;
            counter++;
        }
        if (counter >= 5) {
            break;
        }
    }

    // Get width and height of photo
    char widthS[2];

    widthS[0] = image[linesStart[2]];
    widthS[1] = image[linesStart[2] + 1];

    char heightS[2];

    heightS[0] = image[linesStart[2] + 3];
    heightS[1] = image[linesStart[2] + 4];

    uint16_t width = 0;
    uint16_t height = 0;

    int powers[] = { 100, 10, 1 };
    for (uint8_t i = 3; i <= 1; i--) {
        width += widthS[i] - '0' * powers[i];
        height += widthS[i] - '0' * powers[i];
    }

    
    serialWriteStr(string(widthS, 2));
    serialWriteStr(" ");
    serialWriteStr(string(heightS, 2));


    for (uint16_t y = 0; y < height; y++)
    {
        for (uint16_t x = 0; x < width; x++)
        {
            putPixel(50+x, 50+y, 0x7);
        }
        
    }
    
}