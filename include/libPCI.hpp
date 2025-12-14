#pragma once
#include <stdint.h>
uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t pciConfigRead32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

namespace PCI {
enum HeaderType { GENERAL, PCI_PCI_BRIDGE, PCI_CARDBUS_BRIDGE };

struct statusRegister {
    // https://wiki.osdev.org/PCI
    // Detected Parity Error - This bit will be set to 1 whenever the device detects a parity error, even if parity error handling is disabled.
    // Signalled System Error - This bit will be set to 1 whenever the device asserts SERR#.
    // Received Master Abort - This bit will be set to 1, by a master device, whenever its transaction (except for Special Cycle transactions) is terminated with Master-Abort.
    // Received Target Abort - This bit will be set to 1, by a master device, whenever its transaction is terminated with Target-Abort.
    // Signalled Target Abort - This bit will be set to 1 whenever a target device terminates a transaction with Target-Abort.
    // DEVSEL Timing - Read only bits that represent the slowest time that a device will assert DEVSEL# for any bus command except Configuration Space read and writes. Where a value of 0x0 represents fast timing, a value of 0x1 represents medium timing, and a value of 0x2 represents slow timing.
    // Master Data Parity Error - This bit is only set when the following conditions are met. The bus agent asserted PERR# on a read or observed an assertion of PERR# on a write, the agent setting the bit acted as the bus master for the operation in which the error occurred, and bit 6 of the Command register (Parity Error Response bit) is set to 1.
    // Fast Back-to-Back Capable - If set to 1 the device can accept fast back-to-back transactions that are not from the same agent; otherwise, transactions can only be accepted from the same agent.
    // Bit 6 - As of revision 3.0 of the PCI Local Bus specification this bit is reserved. In revision 2.1 of the specification this bit was used to indicate whether or not a device supported User Definable Features.
    // 66 MHz Capable - If set to 1 the device is capable of running at 66 MHz; otherwise, the device runs at 33 MHz.
    // Capabilities List - If set to 1 the device implements the pointer for a New Capabilities Linked list at offset 0x34; otherwise, the linked list is not available.
    // Interrupt Status - Represents the state of the device's INTx# signal. If set to 1 and bit 10 of the Command register (Interrupt Disable bit) is set to 0 the signal will be asserted; otherwise, the signal will be ignored.
    uint8_t reserved1 : 3;
    bool interrupt_status : 1;
    bool capabilities_list : 1;
    bool capable_66mhz : 1;
    bool reserved2 : 1;
    bool fast_b2b_compatable : 1;
    bool master_data_parity_error : 1;
    uint8_t DEVSEL_timing : 2;
    bool signaled_target_abort : 1;
    bool received_target_abort : 1;
    bool received_master_abort : 1;
    bool signaled_system_error : 1;
    bool detected_parity_error : 1;
};

struct commandRegister {
    // https://wiki.osdev.org/PCI
    // Interrupt Disable - If set to 1 the assertion of the devices INTx# signal is disabled; otherwise, assertion of the signal is enabled.
    // Fast Back-Back Enable - If set to 1 indicates a device is allowed to generate fast back-to-back transactions; otherwise, fast back-to-back transactions are only allowed to the same agent.
    // SERR# Enable - If set to 1 the SERR# driver is enabled; otherwise, the driver is disabled.
    // Bit 7 - As of revision 3.0 of the PCI local bus specification this bit is hardwired to 0. In earlier versions of the specification this bit was used by devices and may have been hardwired to 0, 1, or implemented as a read/write bit.
    // Parity Error Response - If set to 1 the device will take its normal action when a parity error is detected; otherwise, when an error is detected, the device will set bit 15 of the Status register (Detected Parity Error Status Bit), but will not assert the PERR# (Parity Error) pin and will continue operation as normal.
    // VGA Palette Snoop - If set to 1 the device does not respond to palette register writes and will snoop the data; otherwise, the device will trate palette write accesses like all other accesses.
    // Memory Write and Invalidate Enable - If set to 1 the device can generate the Memory Write and Invalidate command; otherwise, the Memory Write command must be used.
    // Special Cycles - If set to 1 the device can monitor Special Cycle operations; otherwise, the device will ignore them.
    // Bus Master - If set to 1 the device can behave as a bus master; otherwise, the device can not generate PCI accesses.
    // Memory Space - If set to 1 the device can respond to Memory Space accesses; otherwise, the device's response is disabled.
    // I/O Space - If set to 1 the device can respond to I/O Space accesses; otherwise, the device's response is disabled.
    bool io_space : 1;
    bool memory_space : 1;
    bool bus_master : 1;
    bool special_cycles : 1;
    bool mem_write_inval_ena : 1;
    bool VGA_palette_snoop : 1;
    bool parity_error_response : 1;
    bool reserved : 1;
    bool SERR_ena : 1;
    bool fast_b2b_ena : 1;
    bool interrupt_disable : 1;
    uint8_t reserved2 : 5;
};

struct configSpace {
    uint16_t deviceID;
    uint16_t vendorID;
    statusRegister status;
    commandRegister command;
    uint8_t classCode;
    uint8_t subclass;
    uint8_t progIF;
    uint8_t revisionID;
    uint8_t BIST;
    bool multiFunction;
    HeaderType headerType;
    uint8_t latencyTimer;
    uint8_t cacheLineSize;
};

}  // namespace PCI

void pciGetConfigSpace(PCI::configSpace* config, uint8_t bus, uint8_t slot, uint8_t func);