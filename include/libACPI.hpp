#pragma once
#include <stdint.h>

struct rsdtDescriptor {
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
struct rsdpDescriptor {
    char signature[8];
    uint8_t checksum;
    char oemId[6];
    uint8_t revision;
    rsdtDescriptor* rsdtAddress;
} __attribute__((packed));

struct rsdpDescriptor20 {
    rsdpDescriptor firstPart;
    uint32_t length;
    uint64_t xsdtAddress;
    uint8_t extendedChecksum;
    uint8_t reserved[3];
} __attribute__((packed));

rsdpDescriptor* findRSDP();
void decodeRSDP(rsdpDescriptor* ptr);
void decodeRSDT(rsdtDescriptor* ptr);

extern rsdpDescriptor* rsdpPtr;
extern rsdtDescriptor* rsdtPtr;