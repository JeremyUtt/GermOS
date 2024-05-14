#include <libACPI.hpp>
#include <libSerial.hpp>
#include <utils.hpp>

rsdpDescriptor* rsdpPtr;
rsdtDescriptor* rsdtPtr;

bool check(char str[], int address) {
    for (uint32_t i = 0; i < getStrLen(str); i++) {
        if (*(char*)(address + i) != str[i]) {
            return false;
        }
    }
    return true;
}

rsdpDescriptor* findRSDP() {
    char str[] = "RSD PTR ";
    int startAddr = 0x000E0000;
    int i = 0;
    // int finds = 0;
    while (true) {
        if (check(str, startAddr + i)) {
            rsdpPtr = (rsdpDescriptor*)(startAddr + i);
            return (rsdpDescriptor*)(startAddr + i);
        }

        i++;
    }
}

void decodeRSDP(rsdpDescriptor* rsdpPointer) {
    rsdtPtr = rsdpPointer->rsdtAddress;

    serialWriteStr("\r\nRSDP Header: ");

    serialWriteStr("\r\n\tsignature: ");
    serialWriteStr(string(rsdpPointer->signature, 8));
    serialWriteStr("\r\n\tchecksum: ");
    serialWriteStr(intToStr(rsdpPointer->checksum, 10));
    serialWriteStr("\r\n\tOEMID: ");
    serialWriteStr(rsdpPointer->oemId);
    serialWriteStr("\r\n\trevision: ");
    serialWriteStr(intToStr(rsdpPointer->revision, 10));
    serialWriteStr("\r\n\tRsdtAddress: ");
    serialWriteStr(intToStr((int)rsdpPointer->rsdtAddress, 16));
}

void decodeRSDT(rsdtDescriptor* rsdtPointer) {
    serialWriteStr("\r\nRSDT Header: ");

    serialWriteStr("\r\n\tSignature: ");
    serialWriteStr(string(rsdtPointer->signature, 8));
    serialWriteStr("\r\n\tLength: ");
    serialWriteStr(intToStr(rsdtPointer->length, 10));
    serialWriteStr("\r\n\tRevision: ");
    serialWriteStr(intToStr(rsdtPointer->revision, 10));
    serialWriteStr("\r\n\tChecksum: ");
    serialWriteStr(intToStr(rsdtPointer->checksum, 10));
    serialWriteStr("\r\n\tOEMID: ");
    serialWriteStr(rsdtPointer->oemId);
    serialWriteStr("\r\n\tOEMTableID: ");
    serialWriteStr(rsdtPointer->oemTableID);
    serialWriteStr("\r\n\tOEMRevision: ");
    serialWriteStr(intToStr(rsdtPointer->oemRevision, 10));
    serialWriteStr("\r\n\tCreatorID: ");
    serialWriteStr(intToStr(rsdtPointer->creatorID, 10));
    serialWriteStr("\r\n\tCreatorRevision: ");
    serialWriteStr(intToStr(rsdtPointer->creatorRevision, 10));
}