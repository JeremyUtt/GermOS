#include <converts.hpp>
#include <libGUI.hpp>
#include <libIO.hpp>
#include <libPCI.hpp>

int pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) |
                         (offset & 0xFC) | ((uint32_t)0x80000000));

    // Write out the address
    outb(0xCF8, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = ((inb(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}
