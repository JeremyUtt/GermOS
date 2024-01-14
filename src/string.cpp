#include "converts.hpp"
#include "string.hpp"

string::string(char* content) {
    _size = getStrLen(content);
    _content = content;
}

string::string(unsigned int size) {
    _size = size;
}

string& string::operator=(char* str){
    _size = getStrLen(str);
    _content = str;
    return *this;
}

string& string::operator=(const char* str){
    _size = getStrLen(str);
    _content = (char*)str;
    return *this;
}


char* string::str(){
    _content[_size] = 0;
    return _content;
}