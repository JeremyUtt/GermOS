#pragma once
#include <libGUI.hpp>
#include <libSerial.hpp>
#include <string.hpp>

enum stream {
    Serial,
    Screen,
};

int sprintf(stream serial, string format);
int printValue(stream serial, char c);
int printValue(stream serial, string& s);
int printValue(stream serial, const char* s);
int printValue(stream serial, int i);

template <typename T, typename... Args>
int sprintf(stream serial, string format, T value, Args... args) {
    int characters = 0;
    for (uint32_t i = 0; i < format.size(); i++) {
        if (format[i] == '%' && format[i + 1] == '%') {
            characters += printValue(serial, '%');
            i++;  // Skip both '%'
        } else if (format[i] == '%') {
            characters += printValue(serial, value);

            if (i + 2 >= format.size()) {
                return characters;
            }

            string newFormat(&format[i + 2]);
            return characters + sprintf(serial, newFormat, args...);

        } else {
            // Just print regular characters
            characters += printValue(serial, format[i]);
            ;
        }
    }
    return characters;
}

int printf(string format);

template <typename... Args>
int printf(string format, Args... args) {
    return sprintf(Screen, format, args...);
}
