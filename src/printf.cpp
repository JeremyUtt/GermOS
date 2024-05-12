#include "printf.hpp"

#include <converts.hpp>

int sprintf(stream serial, string format) {
    return printValue(serial, format);
}

int printf(string format){
    return printValue(Screen, format);
}


#ifdef TEXT_MODE
using namespace TextRenderer;
#else
using namespace GuiRenderer;
#endif

int printValue(stream serial, char c) {
    if (serial == Screen) {
        if (c == '\n') {
            println("");
            return 1;
        }
        printChar(c);
        return 1;
    }

    if (c == '\n') {
        serialWriteStr("\r\n");
        return 2;
    }

    serialWriteChar(c);
    return 1;
}

int printValue(stream serial, string& s) {
    if (serial == Screen) {
        print(s);
    } else {
        serialWriteStr(s);
    }

    return s.size();
}

int printValue(stream serial, const char* s) {
    if (serial == Screen) {
        print(s);
    } else {
        serialWriteStr(s);
    }

    return getStrLen(s);
}

int printValue(stream serial, int i) {
    if (serial == Screen) {
        print(intToStr(i, 10));
    } else {
        serialWriteStr(intToStr(i, 10));
    }

    return intToStr(i, 10).size();
}
