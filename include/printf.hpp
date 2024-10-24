#pragma once
#include <libGUI.hpp>
#include <libSerial.hpp>
#include <string.hpp>

#ifndef NULL
#define NULL 0
#endif
/**
 * @brief Updates where printf will print to
 *
 * @param renderer renderer to print to
 */
void updateStdout(Renderer& renderer);

enum stream {
    Serial,
    Screen,
};

int fprintf(stream serial, string format);
int printValue(stream serial, char c, int unused);
int printValue(stream serial, string& s, int unused);
int printValue(stream serial, const char* s, int unused);
int printValue(stream serial, int i, int base);

template <typename T, typename... Args>
int fprintf(stream serial, string format, T value, Args... args) {
    int characters = 0;
    for (uint32_t i = 0; i < format.size(); i++) {
        if (format[i] == '%') {
            if (format[i + 1] == '%') {
                characters += printValue(serial, '%', NULL);
                i++;  // Skip both '%'
                break;
            }

            // Switch case for different integer types
            switch (format[i + 1]) {
                case 'x':
                case 'X':
                    characters += printValue(serial, value, 16);
                    break;
                default:
                    characters += printValue(serial, value, NULL);
                    break;
            }

            if (i + 2 >= format.size()) {
                return characters;
            }

            string newFormat(&format[i + 2]);
            return characters + fprintf(serial, newFormat, args...);

        } else {
            // Just print regular characters
            characters += printValue(serial, format[i], NULL);
        }
    }
    return characters;
}

int printf(string format);

template <typename... Args>
int printf(string format, Args... args) {
    return fprintf(Screen, format, args...);
}
