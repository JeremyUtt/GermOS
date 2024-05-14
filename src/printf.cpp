#include "printf.hpp"

#include <utils.hpp>

int sprintf(stream serial, string format) {
    return printValue(serial, format);
}

int printf(string format) {
    return printValue(Screen, format);
}

// Node: Polymorphism doesn't work yet in this environment
#ifdef TEXT_MODE
TuiTextRenderer* output;
void updateStdout(TuiTextRenderer& renderer) {
    output = &renderer;
}
#else
GuiTextRenderer* output;
void updateStdout(GuiTextRenderer& renderer) {
    output = &renderer;
}
#endif

int printValue(stream serial, char c) {
    if (serial == Screen) {
        output->printChar(c);
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
        output->print(s);
    } else {
        serialWriteStr(s);
    }

    return s.size();
}

int printValue(stream serial, const char* s) {
    if (serial == Screen) {
        output->print(s);
    } else {
        serialWriteStr(s);
    }

    return getStrLen(s);
}

int printValue(stream serial, int i) {
    if (serial == Screen) {
        output->print(intToStr(i, 10));
    } else {
        serialWriteStr(intToStr(i, 10));
    }

    return intToStr(i, 10).size();
}
