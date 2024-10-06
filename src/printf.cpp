#include "printf.hpp"

#include <utils.hpp>

int fprintf(stream serial, string format) {
    return printValue(serial, format, NULL);
}

int printf(string format) {
    return printValue(Screen, format, NULL);
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

int printValue(stream serial, char c, int unused) {
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

int printValue(stream serial, string& s, int unused) {
    if (serial == Screen) {
        output->print(s);
    } else {
        serialWriteStr(s);
    }

    return s.size();
}

int printValue(stream serial, const char* s, int unused) {
    if (serial == Screen) {
        output->print(s);
    } else {
        serialWriteStr(s);
    }

    return getStrLen(s);
}

int printValue(stream serial, int i, int base) {
    if (base == NULL) {
        base = 10;
    }

    if (serial == Screen) {
        output->print(intToStr(i, base));
    } else {
        serialWriteStr(intToStr(i, base));
    }

    return intToStr(i, 10).size();
}
