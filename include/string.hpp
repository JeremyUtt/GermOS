#pragma once
#include <stdint.h>
class string {
  private:
    char* _content;
    uint32_t _size;

  public:
    string(char* content);
    string(const char* content);
    string(uint32_t size);
    string(char* content, uint32_t size);
    string& operator=(char* str);
    string& operator=(const char* str);
    uint32_t size();
    char* str();
    char at(uint32_t index);
};