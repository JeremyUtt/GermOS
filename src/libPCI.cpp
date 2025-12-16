#include <libIO.hpp>
#include <libPCI.hpp>
#include <printf.hpp>
#include <utils.hpp>
uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    // Write out the address
    outl(0xCF8, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

uint32_t pciConfigRead32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint32_t tmp = 0;

    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    // Write out the address
    outl(0xCF8, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = inl(0xCFC);

    return tmp;
}

inline uint16_t lower16(uint32_t x) {
    return (uint16_t)(x & 0xFFFF);
}

inline uint16_t upper16(uint32_t x) {
    return (uint16_t)(x >> 16) & 0xFFFF;
}

void pciPrintConfigSpace(const PCI::configSpace* cfg) {
    if (!cfg)
        return;

    fprintf(Serial, "  Vendor ID                   : 0x%x\n", (unsigned)cfg->vendorID);
    fprintf(Serial, "  Device ID                   : 0x%x\n", (unsigned)cfg->deviceID);

    fprintf(Serial, "  Command                     :\n");
    fprintf(Serial, "    io_space                  : %u\n", (unsigned)cfg->command.io_space);
    fprintf(Serial, "    memory_space              : %u\n", (unsigned)cfg->command.memory_space);
    fprintf(Serial, "    bus_master                : %u\n", (unsigned)cfg->command.bus_master);
    fprintf(Serial, "    special_cycles            : %u\n", (unsigned)cfg->command.special_cycles);
    fprintf(Serial, "    memory_write_and_inv      : %u\n", (unsigned)cfg->command.mem_write_inval_ena);
    fprintf(Serial, "    vga_palette_snoop         : %u\n", (unsigned)cfg->command.VGA_palette_snoop);
    fprintf(Serial, "    parity_error_response     : %u\n", (unsigned)cfg->command.parity_error_response);
    fprintf(Serial, "    wait_cycle_control (res)  : %u\n", (unsigned)cfg->command.reserved);
    fprintf(Serial, "    serr#_enable              : %u\n", (unsigned)cfg->command.SERR_ena);
    fprintf(Serial, "    fast_back_to_back         : %u\n", (unsigned)cfg->command.fast_b2b_ena);
    fprintf(Serial, "    interrupt_disable         : %u\n", (unsigned)cfg->command.interrupt_disable);

    fprintf(Serial, "  Status                      :\n");
    fprintf(Serial, "    capabilities_list         : %u\n", (unsigned)cfg->status.capabilities_list);
    fprintf(Serial, "    _66mhz_capable            : %u\n", (unsigned)cfg->status.capable_66mhz);
    fprintf(Serial, "    fast_back_to_back_capable : %u\n", (unsigned)cfg->status.fast_b2b_compatable);
    fprintf(Serial, "    devsel_timing             : 0x%x\n", (unsigned)(cfg->status.DEVSEL_timing));
    fprintf(Serial, "    signaled_target_abort     : %u\n", (unsigned)cfg->status.signaled_target_abort);
    fprintf(Serial, "    received_target_abort     : %u\n", (unsigned)cfg->status.received_target_abort);
    fprintf(Serial, "    received_master_abort     : %u\n", (unsigned)cfg->status.received_master_abort);
    fprintf(Serial, "    signaled_system_error     : %u\n", (unsigned)cfg->status.signaled_system_error);
    fprintf(Serial, "    detected_parity_error     : %u\n", (unsigned)cfg->status.detected_parity_error);
    fprintf(Serial, "    interrupt_status          : %u\n", (unsigned)cfg->status.interrupt_status);
    fprintf(Serial, "    master_data_parity_error  : %u\n", (unsigned)cfg->status.master_data_parity_error);

    fprintf(Serial, "  Revision                    : 0x%x\n", (unsigned)cfg->revisionID);
    fprintf(Serial, "  Prog IF                     : 0x%x\n", (unsigned)cfg->progIF);
    fprintf(Serial, "  Subclass                    : 0x%x\n", (unsigned)cfg->subclass);
    fprintf(Serial, "  Class                       : 0x%x\n", (unsigned)cfg->classCode);
    fprintf(Serial, "  CacheLine                   : %u\n", (unsigned)cfg->cacheLineSize);
    fprintf(Serial, "  Latency                     : %u\n", (unsigned)cfg->latencyTimer);
    fprintf(Serial, "  HeaderType                  : 0x%x\n", (unsigned)cfg->headerType);
    fprintf(Serial, "  Multifunc                   : %u\n", (unsigned)cfg->multiFunction);

    fprintf(Serial, "  BIST                        :\n");
    fprintf(Serial, "    start_bist                : %u\n", (unsigned)cfg->BIST.startBIST);
    fprintf(Serial, "    bist_completion_code      : 0x%x\n", (unsigned)(cfg->BIST.completionCode));
    fprintf(Serial, "    bist_capable              : %u\n", (unsigned)cfg->BIST.bistCapable);
}

bool pciGetConfigSpace(PCI::configSpace* config, uint8_t bus, uint8_t slot, uint8_t func) {
    uint32_t array[64];

    for (size_t i = 0; i < 64; i++) {
        uint32_t word = pciConfigRead32(bus, slot, func, i * 4);
        array[i] = word;
    }

    if (upper16(array[0]) == 0xFFFF) {
        return false;
    }
    memcpy(array, config, sizeof(*config));

    return true;
}

// Manual Attempt
bool pciGetConfigSpace_2(PCI::configSpace* config, uint8_t bus, uint8_t slot, uint8_t func) {
    uint32_t array[64];

    for (size_t i = 0; i < 64; i++) {
        uint32_t word = pciConfigRead32(bus, slot, func, i * 4);
        array[i] = word;
    }

    if (upper16(array[0]) == 0xFFFF) {
        return false;
    }

    config->vendorID = lower16(array[0]);
    config->deviceID = upper16(array[0]);

    uint16_t tmp = lower16(array[1]);
    memcpy(&tmp, &(config->command), sizeof(tmp));

    tmp = upper16(array[1]);
    memcpy(&tmp, &(config->status), sizeof(tmp));

    config->revisionID = (array[2] >> 0) & 0x00ff;
    config->progIF = (array[2] >> 8) & 0x00ff;
    config->subclass = (array[2] >> 16) & 0x00ff;
    config->classCode = (array[2] >> 24) & 0x00ff;

    config->cacheLineSize = (array[3] >> 0) & 0x00ff;
    config->latencyTimer = (array[3] >> 8) & 0x00ff;
    config->headerType = (PCI::HeaderType)((array[3] >> 16) & 0x00ff);

    tmp = (array[3] >> 24) & 0x00ff;
    memcpy(&tmp, &(config->BIST), sizeof(tmp));
    return true;
}

// uint16_t pciCheckVendor(uint8_t bus, uint8_t slot) {
//     uint16_t vendor, device;
//     /* Try and read the first configuration register. Since there are no
//      * vendors that == 0xFFFF, it must be a non-existent device. */
//     if ((vendor = pciConfigReadWord(bus, slot, 0, 0)) != 0xFFFF) {
//         device = pciConfigReadWord(bus, slot, 0, 2);
//     }
//     return (vendor);
// }