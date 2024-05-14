#pragma once
#include <stdint.h>
class string {
  private:
    char* _content;
    uint32_t _size;
    bool _dynamic;

  public:
    string();
    string(string& str);
    ~string();
    string& operator=(string& str);

    string(char* content);
    string(const char* content);
    string(uint32_t size);
    string(char* content, uint32_t size);

    string& operator=(char* str);
    string& operator=(const char* str);
    bool operator==(string& str);
    bool operator==(const char* str);
    char& operator[](uint32_t index);
    string operator+(string& rhs);

    uint32_t size() const;
    char* cstr();
    char& at(uint32_t index);
};