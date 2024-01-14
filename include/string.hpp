#pragma once

class string {
private:
    char* _content;
    unsigned int _size;

public:
    string(char* content);
    string(unsigned int size);
    string& operator=(char* str);
    string& operator=(const char* str);
    char* str();
};