#include "classTest.hpp"

#include <memory.hpp>
void classTest::pureVirtualFunction() {
    // Do nothing
    printf("pureVirtualFunction\n");
}

void classTest::virtualFunction() {
    // Do nothing
    printf("virtualFunction\n");
}

void classTest::additionalFunction() {
    // Do nothing
    printf("additionalFunction\n");
}

void testStuff() {
    classTest test;
    test.pureVirtualFunction();
    test.virtualFunction();
    test.normalFunction();
    test.additionalFunction();

    // abstractClass* test2 = &test;
    // test2->pureVirtualFunction();
    // test2->virtualFunction();
    // test2->normalFunction();

    abstractClass& test3 = test;
    test3.pureVirtualFunction();
    test3.virtualFunction();
    test3.normalFunction();
}