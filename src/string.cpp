#include "string.hpp"

#include "converts.hpp"

string::string() {
    _size = 0;
    _content = nullptr;
}

string::string(char* content) {
    _size = getStrLen(content);
    _content = content;
}

string::string(const char* content) {
    _size = getStrLen(content);
    _content = (char*)content;
}

string::string(char* content, uint32_t size) {
    _size = size;
    _content = content;
}

string::string(uint32_t size) {
    _size = size;
}

string& string::operator=(char* str) {
    _size = getStrLen(str);
    _content = str;
    return *this;
}

string& string::operator=(const char* str) {
    _size = getStrLen(str);
    _content = (char*)str;
    return *this;
}

char* string::str() {
    _content[_size] = 0;
    return _content;
}

uint32_t string::size() {
    return _size;
}

char string::at(uint32_t index) {
    if (index >= 0 && index < _size) {
        return _content[index];
    }
    return -1;
}