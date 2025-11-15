#pragma once
#include <stdint.h>

namespace ACPI {

struct SDT_header {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemId[6];
    char oemTableID[8];
    uint32_t oemRevision;
    uint32_t creatorID;
    uint32_t creatorRevision;
};

struct RSDT_full {
    SDT_header header;
    SDT_header* tables[4];
} __attribute__((packed));

struct RSDP {
    char signature[8];
    uint8_t checksum;
    char oemId[6];
    uint8_t revision;
    RSDT_full* rsdtAddress;
} __attribute__((packed));

}  // namespace ACPI

ACPI::RSDP* findRSDP();
void decodeRSDP(ACPI::RSDP* ptr);
void printSDT(ACPI::SDT_header* ptr);
void decode();

extern ACPI::RSDP* rsdpPtr;
extern ACPI::SDT_header* rsdtPtr;