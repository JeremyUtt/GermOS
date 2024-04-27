#include <iostream>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <string>

struct RGB {
    uint8_t r, g, b;
};

const RGB palette[] = {
    {0, 0, 0},       {0, 2, 170},     {20, 170, 0},    {0, 170, 170},   {170, 0, 3},
    {170, 0, 170},   {170, 85, 0},    {170, 170, 170}, {85, 85, 85},    {85, 85, 255},
    {85, 255, 85},   {85, 255, 255},  {255, 85, 85},   {253, 85, 255},  {255, 255, 85},
    {255, 255, 255}, {0, 0, 0},       {16, 16, 16},    {32, 32, 32},    {53, 53, 53},
    {69, 69, 69},    {85, 85, 85},    {101, 101, 101}, {117, 117, 117}, {138, 138, 138},
    {154, 154, 154}, {170, 170, 170}, {186, 186, 186}, {202, 202, 202}, {223, 223, 223},
    {239, 239, 239}, {255, 255, 255}, {0, 4, 255},     {65, 4, 255},    {130, 3, 255},
    {190, 2, 255},   {253, 0, 255},   {254, 0, 190},   {255, 0, 130},   {255, 0, 65},
    {255, 0, 8},     {255, 65, 5},    {255, 130, 0},   {255, 190, 0},   {255, 255, 0},
    {190, 255, 0},   {130, 255, 0},   {65, 255, 1},    {36, 255, 0},    {34, 255, 66},
    {29, 255, 130},  {18, 255, 190},  {0, 255, 255},   {0, 190, 255},   {1, 130, 255},
    {0, 65, 255},    {130, 130, 255}, {158, 130, 255}, {190, 130, 255}, {223, 130, 255},
    {253, 130, 255}, {254, 130, 223}, {255, 130, 190}, {255, 130, 158}, {255, 130, 130},
    {255, 158, 130}, {255, 190, 130}, {255, 223, 130}, {255, 255, 130}, {223, 255, 130},
    {190, 255, 130}, {158, 255, 130}, {130, 255, 130}, {130, 255, 158}, {130, 255, 190},
    {130, 255, 223}, {130, 255, 255}, {130, 223, 255}, {130, 190, 255}, {130, 158, 255},
    {186, 186, 255}, {202, 186, 255}, {223, 186, 255}, {239, 186, 255}, {254, 186, 255},
    {254, 186, 239}, {255, 186, 223}, {255, 186, 202}, {255, 186, 186}, {255, 202, 186},
    {255, 223, 186}, {255, 239, 186}, {255, 255, 186}, {239, 255, 186}, {223, 255, 186},
    {202, 255, 187}, {186, 255, 186}, {186, 255, 202}, {186, 255, 223}, {186, 255, 239},
    {186, 255, 255}, {186, 239, 255}, {186, 223, 255}, {186, 202, 255}, {1, 1, 113},
    {28, 1, 113},    {57, 1, 113},    {85, 0, 113},    {113, 0, 113},   {113, 0, 85},
    {113, 0, 57},    {113, 0, 28},    {113, 0, 1},     {113, 28, 1},    {113, 57, 0},
    {113, 85, 0},    {113, 113, 0},   {85, 113, 0},    {57, 113, 0},    {28, 113, 0},
    {9, 113, 0},     {9, 113, 28},    {6, 113, 57},    {3, 113, 85},    {0, 113, 113},
    {0, 85, 113},    {0, 57, 113},    {0, 28, 113},    {57, 57, 113},   {69, 57, 113},
    {85, 57, 113},   {97, 57, 113},   {113, 57, 113},  {113, 57, 97},   {113, 57, 85},
    {113, 57, 69},   {113, 57, 57},   {113, 69, 57},   {113, 85, 57},   {113, 97, 57},
    {113, 113, 57},  {97, 113, 57},   {85, 113, 57},   {69, 113, 58},   {57, 113, 57},
    {57, 113, 69},   {57, 113, 85},   {57, 113, 97},   {57, 113, 113},  {57, 97, 113},
    {57, 85, 113},   {57, 69, 114},   {81, 81, 113},   {89, 81, 113},   {97, 81, 113},
    {105, 81, 113},  {113, 81, 113},  {113, 81, 105},  {113, 81, 97},   {113, 81, 89},
    {113, 81, 81},   {113, 89, 81},   {113, 97, 81},   {113, 105, 81},  {113, 113, 81},
    {105, 113, 81},  {97, 113, 81},   {89, 113, 81},   {81, 113, 81},   {81, 113, 90},
    {81, 113, 97},   {81, 113, 105},  {81, 113, 113},  {81, 105, 113},  {81, 97, 113},
    {81, 89, 113},   {0, 0, 66},      {17, 0, 65},     {32, 0, 65},     {49, 0, 65},
    {65, 0, 65},     {65, 0, 50},     {65, 0, 32},     {65, 0, 16},     {65, 0, 0},
    {65, 16, 0},     {65, 32, 0},     {65, 49, 0},     {65, 65, 0},     {49, 65, 0},
    {32, 65, 0},     {16, 65, 0},     {3, 65, 0},      {3, 65, 16},     {2, 65, 32},
    {1, 65, 49},     {0, 65, 65},     {0, 49, 65},     {0, 32, 65},     {0, 16, 65},
    {32, 32, 65},    {40, 32, 65},    {49, 32, 65},    {57, 32, 65},    {65, 32, 65},
    {65, 32, 57},    {65, 32, 49},    {65, 32, 40},    {65, 32, 32},    {65, 40, 32},
    {65, 49, 32},    {65, 57, 33},    {65, 65, 32},    {57, 65, 32},    {49, 65, 32},
    {40, 65, 32},    {32, 65, 32},    {32, 65, 40},    {32, 65, 49},    {32, 65, 57},
    {32, 65, 65},    {32, 57, 65},    {32, 49, 65},    {32, 40, 65},    {45, 45, 65},
    {49, 45, 65},    {53, 45, 65},    {61, 45, 65},    {65, 45, 65},    {65, 45, 61},
    {65, 45, 53},    {65, 45, 49},    {65, 45, 45},    {65, 49, 45},    {65, 53, 45},
    {65, 61, 45},    {65, 65, 45},    {61, 65, 45},    {53, 65, 45},    {49, 65, 45},
    {45, 65, 45},    {45, 65, 49},    {45, 65, 53},    {45, 65, 61},    {45, 65, 65},
    {45, 61, 65},    {45, 53, 65},    {45, 49, 65},    {0, 0, 0},       {0, 0, 0},
    {0, 0, 0},       {0, 0, 0},       {0, 0, 0},       {0, 0, 0},       {0, 0, 0},
    {0, 0, 0}};

// Define a function to calculate the Euclidean distance between two RGB colors
int distance(const RGB& c1, const RGB& c2) {
    int dr = c1.r - c2.r;
    int dg = c1.g - c2.g;
    int db = c1.b - c2.b;
    return dr * dr + dg * dg + db * db;
}

// Define a function to find the closest color in a 256-color palette
uint8_t findClosestColor(const RGB& inputColor) {
    int minDistance = 2147483647;

    uint8_t closest = 0;
    for (int i = 0; i < 256; ++i) {
        int dist = distance(inputColor, palette[i]);
        if (dist < minDistance) {
            minDistance = dist;
            closest = i;
        }
    }

    return closest;
}


int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "enter filename" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary);
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});

    file.close();


    // Get position of every newline in header
    uint16_t linesStart[5];
    linesStart[0] = 0;
    uint8_t counter = 1;
    for (uint8_t i = 0; i < 255; i++) {
        if (buffer[i] == '\n') {
            linesStart[counter] = i + 1;
            counter++;
        }
        if (counter >= 5) {
            break;
        }
    }

    // Get width and height of photo
    // the third line of the header contains the width and height of the image in ascii, space separated

    // read in each character o line 3 until  a space is found
    // convert the ascii to an integer
    // repeat for the second number

    std::string widthS = "";
    std::string heightS = "";

    for (uint8_t i = linesStart[2]; i < 255; i++) {
        if (buffer[i] == ' ') {
            break;
        }
        widthS += buffer[i];
    }

    for (uint8_t i = linesStart[2] + widthS.size() + 1; i < 255; i++) {
        if (buffer[i] == '\n') {
            break;
        }
        heightS += buffer[i];
    }

    uint16_t photoWidth = std::stoi(widthS);
    uint16_t photoHeight = std::stoi(heightS);


    std::vector<uint8_t> newBuffer(photoWidth * photoHeight);

    //convert pizel data to new RGB mapping
    for (uint16_t y = 0; y < photoHeight; y++) {
        for (uint16_t x = 0; x < photoWidth; x++) {
            uint32_t byteOffset = linesStart[4] + 3 * ((photoWidth * y) + x);
            RGB color = {buffer[byteOffset], buffer[byteOffset + 1], buffer[byteOffset + 2]};

            // store closest color in new vector
            newBuffer[photoWidth * y + x] = findClosestColor(color);
        }
    }

    std::string newFileName = argv[1];
    // remove the file extension
    newFileName = newFileName.substr(0, newFileName.find_last_of('.'));

    std::ofstream outFile(newFileName + ".gimg", std::ios::binary);

    outFile << "GOOP\n";
    
    // output width and height of image in binary (Not ascii)
    outFile.write(reinterpret_cast<const char*>(&photoWidth), sizeof(photoWidth));
    outFile.write(reinterpret_cast<const char*>(&photoHeight), sizeof(photoHeight));

    // print newBuffer to console
    for (uint16_t y = 0; y < photoHeight; y++) {
        for (uint16_t x = 0; x < photoWidth; x++) {
            // output byte to  file in binary (Not ascii)
            outFile.write(reinterpret_cast<const char*>(&newBuffer[photoWidth * y + x]), sizeof(newBuffer[photoWidth * y + x]));
        }
    }

    outFile.close();

    return 0;
}