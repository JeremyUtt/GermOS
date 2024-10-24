#pragma once
#include <printf.hpp>

class abstractClass {
  public:
    virtual void pureVirtualFunction() = 0;
    virtual void virtualFunction();
    void normalFunction() {
        printf("normalFunction from parent\n");
    };
};

class classTest : public abstractClass {
  public:
    void pureVirtualFunction() override;
    void virtualFunction();
    void additionalFunction();
};

void testStuff();