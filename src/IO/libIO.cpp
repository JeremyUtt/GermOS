#include <libIO.hpp>

void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void outw(uint16_t port, uint16_t value) {
    asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

void outl(uint16_t port, uint32_t value) {
    asm volatile("outl %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t returnVal;
    asm volatile("inb %1, %0" : "=a"(returnVal) : "Nd"(port));
    return returnVal;
}

uint32_t inl(uint16_t port) {
    uint32_t returnVal;
    asm volatile("inl %1, %0" : "=a"(returnVal) : "Nd"(port));
    return returnVal;
}

void io_wait() {
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

void pokeb(uint32_t address, uint32_t val) {
    *((volatile uint8_t*)address) = (uint8_t)val;
}

uint8_t peekb(uint32_t address) {
    return *((volatile uint8_t*)address);
}

void pokew(uint32_t address, uint32_t val) {
    *((volatile uint16_t*)address) = (uint16_t)val;
}