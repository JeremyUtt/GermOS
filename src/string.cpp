#include "string.hpp"

#include <error.hpp>
#include <libSerial.hpp>
#include <memory.hpp>

#include "utils.hpp"
string::string() {
    _size = 0;
    _content = nullptr;
    _dynamic = false;
}

string::~string() {
    if (_content != nullptr && _dynamic) {
        free(_content);
    }
}

string::string(string& str) {

    // Make a shallow copy: 

    // this->_content = str._content;
    // this->_size = str._size;
    // this->_dynamic = str._dynamic;
    // return;

    // Make a deep copy:

    _size = str.size();
    _content = (char*)malloc(_size + 1);

    if (_content == nullptr) {
        error("Failed to allocate memory for string", true);
    }

    for (uint32_t i = 0; i < _size; i++) {
        _content[i] = str[i];
    }
    _content[_size] = 0;
    _dynamic = true;
}

string& string::operator=(string& str) {
    _size = str.size();
    if (_content != nullptr && _dynamic) {
        free(_content);
    }
    _content = (char*)malloc(_size + 1);
    for (uint32_t i = 0; i < _size; i++) {
        _content[i] = str[i];
    }
    _dynamic = true;
    return *this;
}

string::string(char* content) {
    _size = getStrLen(content);
    _content = content;
    _dynamic = false;
}

string::string(const char* content) {
    _size = getStrLen(content);
    _content = (char*)content;
    _dynamic = false;
}

string::string(char* content, uint32_t size) {
    _size = size;
    _content = content;
    _dynamic = false;
}

string::string(uint32_t size) {
    _size = size;
    _content = (char*)malloc(_size + 1);
    _dynamic = true;
}

char* string::cstr() {
    _content[_size] = 0;
    return _content;
}

uint32_t string::size() const {
    return _size;
}
char poggers = -1;

char& string::at(uint32_t index) {
    if (index < _size) {
        return _content[index];
    }
    return poggers;
}

string& string::operator=(char* str) {
    _size = getStrLen(str);
    if (_content != nullptr && _dynamic) {
        free(_content);
    }
    _content = str;
    _dynamic = false;
    return *this;
}

string& string::operator=(const char* str) {
    _size = getStrLen(str);
    if (_content != nullptr && _dynamic) {
        free(_content);
    }
    _content = (char*)str;
    _dynamic = false;
    return *this;
}

char& string::operator[](uint32_t index) {
    if (index < _size) {
        return _content[index];
    }
    return poggers;
}

bool string::operator==(string& str) {
    if (_size != str.size()) {
        return false;
    }
    for (uint32_t i = 0; i < _size; i++) {
        if (_content[i] != str[i]) {
            return false;
        }
    }
    return true;
}

bool string::operator==(const char* str) {
    if (_size != getStrLen(str)) {
        return false;
    }
    for (uint32_t i = 0; i < _size; i++) {
        if (_content[i] != str[i]) {
            return false;
        }
    }
    return true;
}

string string::operator+(string& rhs) {
    string newString(_size + rhs.size());
    for (uint32_t i = 0; i < _size; i++) {
        newString[i] = _content[i];
    }
    for (uint32_t i = 0; i < rhs.size(); i++) {
        newString[i + _size] = rhs[i];
    }
    return newString;
}
