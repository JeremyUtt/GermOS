#include <libIO.hpp>
#include <libPCI.hpp>
#include <printf.hpp>
#include <utils.hpp>
#include <dictionary.hpp>
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

bool pciGetConfigSpace(PCI::ConfigSpaceHeader* config, uint8_t bus, uint8_t slot, uint8_t func) {
    // number of 32bit sections in headers
    const int headerSize32bit = 4;

    uint32_t array[headerSize32bit];

    for (size_t i = 0; i < headerSize32bit; i++) {
        uint32_t word = pciConfigRead32(bus, slot, func, i * 4);
        array[i] = word;
    }

    if (upper16(array[0]) == 0xFFFF) {
        return false;
    }
    memcpy(array, config, sizeof(PCI::ConfigSpaceHeader));

    return true;
}

bool pciGetFullConfigSpace(PCI::FullConfigSpace* space, uint8_t bus, uint8_t slot, uint8_t func) {
    bool result = pciGetConfigSpace(&space->header, bus, slot, func);
    if (!result) {
        return false;
    }
    const int extendedConfigSize = 11;
    uint32_t array[extendedConfigSize];

    switch (space->header.headerType) {
        case PCI::GENERAL:

            for (size_t i = 0; i < extendedConfigSize; i++) {
                uint32_t word = pciConfigRead32(bus, slot, func, (i + 4) * 4);
                array[i] = word;
            }
            memcpy(array, &space->device.general, sizeof(PCI::ExtendedConfigSpace));

            break;
        case PCI::PCI_PCI_BRIDGE:
            /* code */
            break;
        case PCI::PCI_CARDBUS_BRIDGE:
            /* code */
            break;
        default:
            return false;
    }
    return true;
}

void pciPrintConfigSpace(const PCI::ConfigSpaceHeader* cfg, stream output) {
    if (!cfg)
        return;

    fprintf(output, "  Vendor ID                   : 0x%x\n", (unsigned)cfg->vendorID);
    fprintf(output, "  Device ID                   : 0x%x\n", (unsigned)cfg->deviceID);

    fprintf(output, "  Command                     :\n");
    fprintf(output, "    io_space                  : %u\n", (unsigned)cfg->command.io_space);
    fprintf(output, "    memory_space              : %u\n", (unsigned)cfg->command.memory_space);
    fprintf(output, "    bus_master                : %u\n", (unsigned)cfg->command.bus_master);
    fprintf(output, "    special_cycles            : %u\n", (unsigned)cfg->command.special_cycles);
    fprintf(output, "    memory_write_and_inv      : %u\n", (unsigned)cfg->command.mem_write_inval_ena);
    fprintf(output, "    vga_palette_snoop         : %u\n", (unsigned)cfg->command.VGA_palette_snoop);
    fprintf(output, "    parity_error_response     : %u\n", (unsigned)cfg->command.parity_error_response);
    fprintf(output, "    wait_cycle_control (res)  : %u\n", (unsigned)cfg->command.reserved);
    fprintf(output, "    serr#_enable              : %u\n", (unsigned)cfg->command.SERR_ena);
    fprintf(output, "    fast_back_to_back         : %u\n", (unsigned)cfg->command.fast_b2b_ena);
    fprintf(output, "    interrupt_disable         : %u\n", (unsigned)cfg->command.interrupt_disable);

    fprintf(output, "  Status                      :\n");
    fprintf(output, "    capabilities_list         : %u\n", (unsigned)cfg->status.capabilities_list);
    fprintf(output, "    _66mhz_capable            : %u\n", (unsigned)cfg->status.capable_66mhz);
    fprintf(output, "    fast_back_to_back_capable : %u\n", (unsigned)cfg->status.fast_b2b_compatable);
    fprintf(output, "    devsel_timing             : 0x%x\n", (unsigned)(cfg->status.DEVSEL_timing));
    fprintf(output, "    signaled_target_abort     : %u\n", (unsigned)cfg->status.signaled_target_abort);
    fprintf(output, "    received_target_abort     : %u\n", (unsigned)cfg->status.received_target_abort);
    fprintf(output, "    received_master_abort     : %u\n", (unsigned)cfg->status.received_master_abort);
    fprintf(output, "    signaled_system_error     : %u\n", (unsigned)cfg->status.signaled_system_error);
    fprintf(output, "    detected_parity_error     : %u\n", (unsigned)cfg->status.detected_parity_error);
    fprintf(output, "    interrupt_status          : %u\n", (unsigned)cfg->status.interrupt_status);
    fprintf(output, "    master_data_parity_error  : %u\n", (unsigned)cfg->status.master_data_parity_error);

    fprintf(output, "  Revision                    : 0x%x\n", (unsigned)cfg->revisionID);
    fprintf(output, "  Prog IF                     : 0x%x\n", (unsigned)cfg->progIF);
    fprintf(output, "  Subclass                    : 0x%x\n", (unsigned)cfg->subclass);
    fprintf(output, "  Class                       : 0x%x\n", (unsigned)cfg->classCode);
    decodeDeviceTypesFunctions(cfg->classCode, cfg->subclass, cfg->progIF);
    fprintf(output, "  CacheLine                   : %u\n", (unsigned)cfg->cacheLineSize);
    fprintf(output, "  Latency                     : %u\n", (unsigned)cfg->latencyTimer);
    fprintf(output, "  HeaderType                  : 0x%x\n", (unsigned)cfg->headerType);
    fprintf(output, "  Multifunc                   : %u\n", (unsigned)cfg->multiFunction);


    fprintf(output, "  BIST                        :\n");
    fprintf(output, "    start_bist                : %u\n", (unsigned)cfg->BIST.startBIST);
    fprintf(output, "    bist_completion_code      : 0x%x\n", (unsigned)(cfg->BIST.completionCode));
    fprintf(output, "    bist_capable              : %u\n", (unsigned)cfg->BIST.bistCapable);
}

void pciPrintFullConfigSpace(const PCI::FullConfigSpace* space, stream output) {
    if (!space)
        return;

    pciPrintConfigSpace(&space->header, output);
    switch (space->header.headerType) {
        case PCI::GENERAL:
            fprintf(output, "  Base Address 0             : 0x%x\n", (unsigned)space->device.general.baseAddr0);
            decodeBaseAddressRegister(space->device.general.baseAddr0);
            fprintf(output, "  Base Address 1             : 0x%x\n", (unsigned)space->device.general.baseAddr1);
            decodeBaseAddressRegister(space->device.general.baseAddr1);
            fprintf(output, "  Base Address 2             : 0x%x\n", (unsigned)space->device.general.baseAddr2);
            decodeBaseAddressRegister(space->device.general.baseAddr2);
            fprintf(output, "  Base Address 3             : 0x%x\n", (unsigned)space->device.general.baseAddr3);
            decodeBaseAddressRegister(space->device.general.baseAddr3);
            fprintf(output, "  Base Address 4             : 0x%x\n", (unsigned)space->device.general.baseAddr4);
            decodeBaseAddressRegister(space->device.general.baseAddr4);
            fprintf(output, "  Base Address 5             : 0x%x\n", (unsigned)space->device.general.baseAddr5);
            decodeBaseAddressRegister(space->device.general.baseAddr5);
            fprintf(output, "  CardBus CIS Pointer         : 0x%x\n", (unsigned)space->device.general.cardBusCISPtr);
            fprintf(output, "  Subsystem Vendor ID         : 0x%x\n", (unsigned)space->device.general.subsystemVendorID);
            fprintf(output, "  Subsystem ID                : 0x%x\n", (unsigned)space->device.general.subsystemID);
            fprintf(output, "  Expansion ROM Base Address   : 0x%x\n", (unsigned)space->device.general.extROMBaseAddr);
            fprintf(output, "  Capabilities Pointer         : 0x%x\n", (unsigned)space->device.general.capabilitiesPtr);
            fprintf(output, "  Interrupt Line              : %u\n", (unsigned)space->device.general.interruptLine);
            fprintf(output, "  Interrupt Pin               : %u\n", (unsigned)space->device.general.interruptPIN);
            fprintf(output, "  Min Grant                   : %u\n", (unsigned)space->device.general.minGrant);
            fprintf(output, "  Max Latency                 : %u\n", (unsigned)space->device.general.maxLatency);

            break;
        case PCI::PCI_PCI_BRIDGE:
            fprintf(output, "  Unimplemented PCI-PCI Bridge Device\n");

            break;
        case PCI::PCI_CARDBUS_BRIDGE:
            fprintf(output, "  Unimplemented PCI-CardBus Bridge Device\n");
            break;
        default:
            fprintf(output, "  Unimplemented Device Type\n");
            break;
    }
}

void pciPrintAllDevices(stream output, int level) {
    for (uint16_t bus = 0; bus < 2; bus++) {
        for (uint16_t slot = 0; slot < 32; slot++) {
            for (uint8_t function = 0; function < 8; function++) {
                PCI::FullConfigSpace space;

                bool result = pciGetFullConfigSpace(&space, bus, slot, function);
                if (function == 0 && !result) {
                    // fprintf(Serial, "Bus %d, Slot %d is Empty\n", bus, slot);
                    break;
                }

                if (result) {
                    fprintf(output, "Bus %d, Slot %d is has function %d\n", bus, slot, function);

                    switch (level) {
                        case 1:
                            fprintf(output, "   Vendor: 0x%x, Device: 0x%x\n", space.header.vendorID, space.header.deviceID);
                            break;
                        case 2:
                            pciPrintConfigSpace(&space.header, output);
                            break;
                        case 3:
                            pciPrintFullConfigSpace(&space, output);
                        default:
                            break;
                    }
                }
            }
        }
    }
}

// Print one compact line for every device found on the PCI bus.
void pciPrintAllDevicesBrief(stream output) {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            for (uint8_t function = 0; function < 8; function++) {
                uint32_t id = pciConfigRead32((uint8_t)bus, slot, function, 0x00);

                if ((id & 0xFFFF) == 0xFFFF) {
                    if (function == 0)
                        break;
                    continue;
                }

                uint32_t classInfo = pciConfigRead32((uint8_t)bus, slot, function, 0x08);
                fprintf(output,
                        "%u:%u.%u ID 0x%x:0x%x Class 0x%x Subclass 0x%x\n",
                        (unsigned)bus, (unsigned)slot, (unsigned)function,
                        (unsigned)(id & 0xFFFF), (unsigned)(id >> 16),
                        (unsigned)((classInfo >> 24) & 0xFF),
                        (unsigned)((classInfo >> 16) & 0xFF));
            }
        }
    }
}

void decodeBaseAddressRegister(uint32_t address) {
    PCI::BaseAddrType baseAddrType = (PCI::BaseAddrType)(address & 0x00000001);

    uint32_t baseAddress;
    fprintf(Serial, "    Address: 0b%s\n", intToStr(address, 2));
    if (baseAddrType == PCI::IO_SPACE) {
        fprintf(Serial, "    Kind: IO\n");
        baseAddress = address & 0xFFFFFFFC;
    } else {
        baseAddress = address & 0xFFFFFFF0;
        fprintf(Serial, "    Kind: Memory\n");
        PCI::MemoryAddrType type = (PCI::MemoryAddrType)((address & 0x00000006) >> 1);
        fprintf(Serial, "    Type: %s\n", type == PCI::BIT32 ? "32 Bits" : "64 Bits");
        bool prefetchble = (address & 0x00000008) >> 3;
        fprintf(Serial, "    Prefetchble: %s\n", prefetchble ? "True" : "False");
    }
    fprintf(Serial, "    True Base Address: 0x%x\n", baseAddress);
}


// static const char* const list[] = {"Unclassified", "MassStorageController", "NetworkController", "DisplayController", "MultiMediaController", "MemoryController", "Bridge", "SimpleCommunicationController", "BaseSystemPeripheral", "InputDeviceController", "DockingStation", "Processor", "SerialBusController", "WirelessController", "IntelligentController", "SatelliteCommunicationController", "EncryptionController", "SignalProcessingController", "ProcessingAccelerator", "NonEssentialInstrumentation"};
void createDict(dictionary* dict) {
    dict->add(0, "Unclassified");
    dict->add(1, "MassStorageController");

    dict->createChildDictionary(1);
    dict->getByKey(1)->child->add(0, "SCSIController");
    dict->getByKey(1)->child->add(1, "IDEController");
    dict->getByKey(1)->child->add(2, "FloppyDiskController");
    dict->getByKey(1)->child->add(3, "IPIBusController");
    dict->getByKey(1)->child->add(4, "RAIDController");
    dict->getByKey(1)->child->add(5, "ATAController");
    dict->getByKey(1)->child->add(6, "SATAController");
    dict->getByKey(1)->child->add(7, "SerialAttachedSCSI(SAS)Controller");
    dict->getByKey(1)->child->add(8, "NonVolatileMemoryController");
    dict->getByKey(1)->child->add(0x80, "Other");

    dict->add(2, "NetworkController");
    dict->createChildDictionary(2);
    dict->getByKey(2)->child->add(0, "EthernetController");
    dict->getByKey(2)->child->add(1, "TokenRingController");
    dict->getByKey(2)->child->add(2, "FDDIController");
    dict->getByKey(2)->child->add(3, "ATMController");
    dict->getByKey(2)->child->add(4, "ISDNController");
    dict->getByKey(2)->child->add(5, "WorldFipController");
    dict->getByKey(2)->child->add(6, "PICMGController");
    dict->getByKey(2)->child->add(7, "InfinibandController");
    dict->getByKey(2)->child->add(8, "FabricController");
    dict->getByKey(2)->child->add(0x80, "Other");


    dict->add(3, "DisplayController");
    dict->createChildDictionary(3);
    dict->getByKey(3)->child->add(0, "VGAController");
    dict->getByKey(3)->child->add(1, "XGAController");
    dict->getByKey(3)->child->add(2, "3DController");
    dict->getByKey(3)->child->add(0x80, "Other");


    dict->add(4, "MultiMediaController");
    dict->add(5, "MemoryController");
    dict->add(6, "Bridge");
    dict->add(7, "SimpleCommunicationController");
    dict->add(8, "BaseSystemPeripheral");
    dict->add(9, "InputDeviceController");
    dict->add(10, "DockingStation");
    dict->add(11, "Processor");
    dict->add(12, "SerialBusController");
    dict->add(13, "WirelessController");
    dict->add(14, "IntelligentController");
    dict->add(15, "SatelliteCommunicationController");
    dict->add(16, "EncryptionController");
    dict->add(17, "SignalProcessingController");
    dict->add(18, "ProcessingAccelerator");
    dict->add(19, "NonEssentialInstrumentation");
    dict->add(0x40, "CoProcessor");
    dict->add(0xFF, "UnassignedClass");

}



void decodeDeviceTypesFunctions(uint8_t classCode, uint8_t subclass, uint8_t progif) {
    dictionary dict;
    createDict(&dict);

    dictionary::node* classNode = dict.getByKey(classCode);
    if (classNode) {
        fprintf(Serial, "    Class Type: %s\n", classNode->value);
        dictionary::node* subclassNode = classNode->child->getByKey(subclass);
        if (subclassNode) {
            fprintf(Serial, "    Subclass Type: %s\n", subclassNode->value);
            dictionary::node* progifNode = subclassNode->child->getByKey(progif);
            if (progifNode) {
                fprintf(Serial, "    Programming Interface Type: %s\n", progifNode->value);
            } else {
                fprintf(Serial, "    Programming Interface Type: Unknown\n");
            }
        } else {
            fprintf(Serial, "    Subclass Type: Unknown\n");
        }
    } else {
        fprintf(Serial, "    Class Type: Unknown\n");
    }
}



